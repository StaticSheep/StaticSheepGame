/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#include "API.h"

#include <glload/gl_4_0.hpp>
#include <glload/gl_load.hpp>
#include <glmesh/glmesh.h>
#include <glutil/glutil.h>

#include "App.h"
#include "Entity.h"
#include "HalfEdgeMesh.h"
#include "MeshOperations.h"
#include "WxWindow.h"
#include "WxInputBackend.h"
#include "gl/WxGLCanvas.h"
#include "BoundingVolume.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

namespace dit {

#pragma region Hw1Window implementation
    
    //-----------------------------------//

    void Hw2Window::OnOpenFile(wxCommandEvent& event)
    {
        wxFileDialog ofd {
        this
        , "Open XYZ file"
        , "", ""
        , "OBJ files (*.obj)|*.obj"
        , wxFD_OPEN | wxFD_FILE_MUST_EXIST };

        auto res = ofd.ShowModal();
        if (res == wxID_CANCEL)
            return;

        String fileName{ ofd.GetPath().c_str() };

        auto loadFn = [=]() {
            log("loading mesh \"%0\"", fileName.c_str());

            auto mesh = MeshFromOBJ(fileName);
            send(core()->root->events, "mesh-loaded", mesh, fileName);
        };

        core()->threads.Post(loadFn);
    }

    //-----------------------------------//

    void Hw2Window::OnExit(wxCommandEvent& event)
    {
        this->Close();
    }

    //-----------------------------------//

    void Hw2Window::OnPropGridChanging(wxPropertyGridEvent& event)
    {
        event.Veto();
    }

    //-----------------------------------//

    void Hw2Window::OnFlagCoordFrame(wxCommandEvent& event)
    {
        send(core()->root->events, "toggle-flag", App::Flags::CoordFrame);
    }

    //-----------------------------------//

    void Hw2Window::OnCheckDrawAABB(wxCommandEvent& event)
    {
        send(core()->root->events, "toggle-flag", App::Flags::DrawAABB);
    }

    //-----------------------------------//

    void Hw2Window::OnCheckDrawBS(wxCommandEvent& event)
    {
        send(core()->root->events, "toggle-flag", App::Flags::DrawBS);
    }

    //-----------------------------------//

    void Hw2Window::OnSelectBSMethod(wxCommandEvent& event)
    {
        send(core()->root->events, "bs-method", String{ event.GetString() });
    }

    //-----------------------------------//

#pragma endregion
#pragma region wxApp overrides

    //-----------------------------------//

    bool App::OnInit()
    {
        // init wx
        if (!wxApp::OnInit())
            return false;

        wxInitAllImageHandlers();

        Init();

        return true;
    }

    //-----------------------------------//

    void App::CleanUp()
    {
        Shutdown();
    }

    //-----------------------------------//

    int32 App::FilterEvent(wxEvent& event)
    {
        if (input == nullptr)
            return -1;

        // keyboard events
        auto type = event.GetEventType();
        if (type == wxEVT_CHAR)
        {
            input->ProcessCharEvent((wxKeyEvent&) event);
            return 1;
        }
        else if (type == wxEVT_KEY_DOWN)
        {
            input->ProcessKeyEvent((wxKeyEvent&) event, true);
            return 1;
        }
        else if (type == wxEVT_KEY_UP)
        {
            input->ProcessKeyEvent((wxKeyEvent&) event, false);
            return 1;
        }

        // mouse events
        static std::array<wxEventType, 13> mouseEvents{ {
            wxEVT_LEFT_DOWN
            , wxEVT_LEFT_UP
            , wxEVT_MIDDLE_DOWN
            , wxEVT_MIDDLE_UP
            , wxEVT_RIGHT_DOWN
            , wxEVT_RIGHT_UP
            , wxEVT_MOTION
            , wxEVT_LEFT_DCLICK
            , wxEVT_MIDDLE_DCLICK
            , wxEVT_RIGHT_DCLICK
            , wxEVT_LEAVE_WINDOW
            , wxEVT_ENTER_WINDOW
            , wxEVT_MOUSEWHEEL } };

        for (auto meType : mouseEvents)
        {
            if (type == meType)
            {
                input->ProcessMouseEvent((wxMouseEvent&) event);
                return -1; // still want to propogate the mouse event to the gui
            }
        }

        return -1;
    }

    //-----------------------------------//

#pragma endregion
#pragma region Common interface

    //-----------------------------------//

    void App::Init()
    {
        flags = { true, false, false, true, false, false };

        // create stuff
        core = new Core{};
        input = new WxInputBackend{};

        // initialize all the things
        core->Init();
        InitGfx();
        InitEvents();

        // get the mouse pos
        auto& ms = input->mouse.state;
        lastMousePos = float2{ ms.x, ms.y };

        // show it
        window->Show(true);
    }

    //-----------------------------------//

    void App::InitGfx()
    {
        using namespace glmesh;

        // create the window
        auto hw2 = new Hw2Window{ nullptr, input };
        int32 width = 0, height = 0;
        hw2->GetClientSize(&width, &height);
        auto gwp = hw2->gameWindowPanel;

        window = new WxWindow{ { (uint16) width, (uint16) height, String{ hw2->GetTitle().c_str() }, hw2 }, hw2, input };

        // create the cavas / gl context
        std::array<int32, 9> glAttrs{ {
            WX_GL_RGBA, 1
            , WX_GL_LEVEL, 0
            , WX_GL_DOUBLEBUFFER, 1
            , WX_GL_DEPTH_SIZE, 32
            , 0
                } };

        window->canvas = new WxGLCanvas{ window, gwp, -1, glAttrs.data(), gwp->GetPosition(), gwp->GetSize(), gwp->GetWindowStyle() };
        window->canvas->Reparent(hw2);

        window->CreateContext();
        window->MakeCurrent();

        // init open gl
        auto glloadRes = glload::LoadFunctions();
        String glloadStatus;
        if (!glloadRes)
            glloadStatus = "failed to initialize";
        else
            glloadStatus = "initialized";

        log("%0 OpenGL %1.%2", glloadStatus.c_str(), glload::GetMajorVersion(), glload::GetMinorVersion());

        // connect to wx events
        window->onUpdateEvent.push_back([&]() {
            Update();
        });

        window->onDrawEvent.push_back([&]() {
            Draw();
        });

        // create the glmesh immediate mode helpers
        AttributeList attrs{ {
                { 0, 3, VDT_SINGLE_FLOAT, ADT_FLOAT }       // position
                , { 1, 3, VDT_SINGLE_FLOAT, ADT_FLOAT }     // normal
                , { 2, 3, VDT_UNSIGN_BYTE, ADT_NORM_FLOAT } // color
                } };

        vtxFmt = new VertexFormat{ attrs };
        stream = new StreamBuffer{ 1024 * 1024 };

        // create the bv unit sphere mesh
        auto bsMeshPtr = glmesh::gen::UnitSphere(32, 32);
        bvMeshes[0].reset(bsMeshPtr);

        // load a shader
        LoadShader("model");
        LoadShader("line");

        // camera stuff
        camera = new Camera{};
        camera->position() = float3{ 5.f, 5.f, 5.f };
        camera->rotation() = lookAt(camera->position(), float3{ 0.f, 0.f, 0.f }, float3{ 0.f, 1.f, 0.f });
        camera->inverse() = inverse(camera->rotation());
    }

    //-----------------------------------//

    void App::InitEvents()
    {
        //-----------------------------------//
        // app events

        // filewatcher stuff
        FileWatcher::Listener fileWatch = [&](WatchID watchid, const String& dir, const String& filename, Action action) {
            this->OnFileWatch(watchid, dir, filename, action);
        };
        core->fileWatcher.Add(shaderBinPath.string(), fileWatch);

        // mesh loading
        std::function<void(HalfEdgeMesh*, String)> onMeshLoaded = [&](HalfEdgeMesh* meshData, String path) {

            if (meshData == nullptr)
            {
                fs::path file{ path };
                log("%0 could not be loaded.", file.filename().string().c_str());
                return;
            }

            // create the model
            auto mdl = std::make_shared<Model>();
            mdl->meshData.reset(meshData);
            
            float3 exts = mdl->meshData->exts[1] - mdl->meshData->exts[0];
            float maxExt = std::max(exts.x, std::max(exts.y, exts.z));
            maxExt = 1.f / maxExt;

            mdl->scale = float3{ maxExt };
            mdl->drawable = BuildMesh(mdl->meshData);

            // populate the ui
            auto wxw = (Hw2Window*) (window->wxw);
            wxw->modelFile->SetValueFromString(wxString{ path });
            wxw->triCount->SetValueFromInt(mdl->meshData->triangles());
            wxw->vertCount->SetValueFromInt(mdl->meshData->vertices());

            // clear & add
            models.clear();
            models.push_back(mdl);

            // create the bounding info
            if (aabb)
            {
                delete aabb;
                aabb = nullptr;
            }
            if (bs)
            {
                delete bs;
                bs = nullptr;
            }

            aabb = new AxisAlignedBoundingBox{ mdl->meshData->exts[0], mdl->meshData->exts[1] };
            bs = new BoundingSphere{ *aabb, false };
        };

        // toggle assignment flags
        std::function<void(App::Flags)> onToggleFlag = [&](App::Flags type) {
            flags[(uint32) type] = !flags[(uint32) type];
        };

        // select the bs method
        std::function<void(String)> onSelectBSMethod = [&](String method) {
            
            assert(bs);

            if (method == "aabb")
            {
                assert(aabb);
                bs->BuildFromAABB(*aabb, false);
            }
            else if (method == "centroid")
            {
                // ...
            }
            else if (method == "ritters")
            {
                // ...
            }
            else if (method == "larsen")
            {
                // ...
            }
        };

        // app event table
        appEvents = {
            { "mesh-loaded", onMeshLoaded }
            , { "toggle-flag", onToggleFlag }
            , { "bs-method", onSelectBSMethod } };

        core->root->events.map = &appEvents;

        //-----------------------------------//
        // input

        // hook up mouse input
        Mouse::MouseDragConsumer onMouseDrag = [&](const MouseDragEvent& e) {
            OnMouseDrag(e);
        };
        Mouse::MouseWheelConsumer onMouseWheel = [&](const MouseWheelEvent& e) {
            OnMouseWheel(e);
        };

        input->mouse.onMouseDrag.push_back(onMouseDrag);
        input->mouse.onMouseWheelMove.push_back(onMouseWheel);

        // hook up the keyboard stuff
        Keyboard::KeyEventConsumer onKeys = [&](const KeyEvent&) {
            OnKeys();
        };

        input->keyboard.onKeyPress.push_back(onKeys);
    }

    //-----------------------------------//

    void App::Shutdown()
    {
        // shutdown / clean up
        models.clear();
        core->Shutdown();

        // destroy resources
        delete stream;
        delete vtxFmt;
        delete camera;
        delete input;
        delete window;
        delete core;

        stream = nullptr;
        vtxFmt = nullptr;
        camera = nullptr;
        input = nullptr;
        window = nullptr;
        core = nullptr;
    }

    //-----------------------------------//

    void App::Update()
    {
        // clear last frame's filewatch events
        watchEvents.clear();

        // calculate normalized mouse coords
        auto& ms = input->mouse.state;
        float mx = ((2.f * ms.x) / 1280) - 1.f;
        float my = 1.f - (2.f * ms.y) / 720;

        lastMousePos = float2{ mx, my };

        // move the camera around
        camera->rotation() = lookAt(camera->position(), float3{ 0.f, 0.f, 0.f }, float3{ 0.f, 1.f, 0.f });
        camera->inverse() = inverse(camera->rotation());

        // pump events from other threads
        core->Update();
    }

    //-----------------------------------//

    void App::OnKeys()
    {
        auto& kb = input->keyboard;

        // drop out on escape
        if (kb.IsKeyDown(Keys::Escape))
            window->wxw->Close();
    }

    //-----------------------------------//

    void App::OnMouseDrag(const MouseDragEvent& e)
    {
        auto& cam = *camera;

        float3 fwd = cam.position() - zero<float3>();
        float3 up{ 0.f, 1.f, 0.f };
        float3 right = normalize(cross(fwd, up));
        float3 pos = cam.position();

        float r = length(fwd);
        float K = std::min(r / 10.f, 1.f) * 10.f;

        auto& ms = input->mouse.state;
        float mx = ((2.f * ms.x) / 1280) - 1.f;
        float my = 1.f - (2.f * ms.y) / 720;

        pos += K * (mx - lastMousePos.x) * right;
        pos += -K * (my - lastMousePos.y) * up;

        cam.position() = r * normalize(pos - zero<float3>());
    }

    //-----------------------------------//

    void App::OnMouseWheel(const MouseWheelEvent& e)
    {
        auto& cam = *camera;
        float3 fwd = cam.position() - zero<float3>();
        float d = length(fwd);
        if (d < .15f && e.delta > 0)
            return;

        cam.position() = cam.position() + (float) e.delta * -.1f * (fwd / d);
    }

    //-----------------------------------//

    void App::Draw()
    {
        auto& cam = *camera;

        gl::LineWidth(2.f);

        // culling
        gl::Enable(gl::CULL_FACE);
        gl::CullFace(gl::BACK);

        // depth buffer
        gl::Enable(gl::DEPTH_TEST);
        gl::DepthFunc(gl::LESS);
        gl::DepthMask(gl::TRUE_);

        // clear
        gl::ClearColor(.1f, .1f, .1f, 1.f);
        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

        auto size = window->canvas->GetSize();
        gl::Viewport(0, 0, size.x, size.y);

        // set up model shader
        auto prog = shaders["model"];
        if (prog == nullptr)
            return;

        gl::UseProgram(prog->id);

        float4x4 view = cam.view();
        float4x4 projection = cam.projection();

        float4x4 wv = view;
        float4x4 wvp = projection * view;
        float3x3 wit = one<float3x3>();

        float4 lightPos{ 100.f, 100.f, 100.f, 1.f };
        float3 Kd{ 1.f, 1.f, 1.f };
        float3 Ld{ 1.f, 1.f, 1.f };

        prog->set("lightViewPos", lightPos);
        prog->set("Kd", Kd);
        prog->set("Ld", Ld);
        prog->set<const float4x4&>("wv", wv);
        prog->set<const float4x4&>("wvp", wvp);
        prog->set<const float3x3&>("wit", wit);

        // draw reticle
        if (flags[(uint32) Flags::CoordFrame])
        {
            using namespace mesh;
            using namespace glmesh;

            float3 eye = zero<float3>();

            std::array<float3, 3> axes{ {
                float3{ 1.f, 0.f, 0.f }
                , float3{ 0.f, 1.f, 0.f }
                , float3{ 0.f, 0.f, -1.f } } };

            std::array<float3, 6> vtx{ {
                eye - 1.f * axes[0]
                , eye + 1.f * axes[0]
                , eye - 1.f * axes[1]
                , eye + 1.f * axes[1]
                , eye - 1.f * axes[2]
                , eye + 1.f * axes[2] } };

            std::array<glmesh::gen::Color, 6> clrs{ {
                glmesh::gen::Color{ 255, 0, 0, 255 }
                , glmesh::gen::Color{ 0, 255, 0, 255 }
                , glmesh::gen::Color{ 0, 0, 255, 255 }
                , glmesh::gen::Color{ 255, 0, 0, 255 }
                , glmesh::gen::Color{ 0, 255, 0, 255 }
                , glmesh::gen::Color{ 0, 0, 255, 255 } } };

            glmesh::Draw drawable{ gl::LINES, vtx.size(), *vtxFmt, *stream };
            for (uint32 i = 0; i < vtx.size(); ++i)
            {
                drawable.Attrib(vtx[i]);
                drawable.Attrib(one<float3>());    // just pass in some value for the normal
                drawable.Attrib<byte>(clrs[i][0], clrs[i][1], clrs[i][2], clrs[i][3]);
            }

            drawable.Render();
        }

        // draw meshes
        for (auto& mdl : models)
        {
            wv = view * mdl->world();
            wvp = projection * wv;

            prog->set<const float4x4&>("wv", wv);
            prog->set<const float4x4&>("wvp", wvp);

            // draw solid mesh
            if (flags[(uint32) Flags::SolidMesh])
                mdl->drawable->Render();

            // draw the holes
            if (flags[(uint32) Flags::DrawHoles])
            {
                gl::Disable(gl::DEPTH_TEST);
                DrawMeshHoles(mdl->meshData);
            }

            // draw the aabb
            if (flags[(uint32) Flags::DrawAABB])
            {
                // ...
            }

            // draw the bounding sphere
            if (flags[(uint32) Flags::DrawBS])
            {
                bvMeshes[0]->Render();
            }
        }
    }

    //-----------------------------------//

    void App::DrawMeshHoles(HalfEdgeMeshPtr mesh)
    {
        using namespace mesh;
        using namespace glmesh;

        // build a list of verts that are individual edges along a hole
        static std::vector<uint32> verts;
        walk(faces(*mesh), [&](const FaceIterator& face) -> FaceIterator {

            auto fedges = edges(face);
            walk(begin(fedges), [&](const EdgeIterator& edge) -> EdgeIterator {

                // if there is no twin
                if (edge.twin() == nullptr)
                {
                    verts.push_back(edge.value().vtx[0]);
                    verts.push_back(edge.value().vtx[1]);
                }

                return next(edge);
            });

            return next(face);
        });

        // submit vertex data to the gpu
        if (verts.empty())
            return;

        glmesh::Draw drawable{ gl::LINES, verts.size(), *vtxFmt, *stream };
        for (auto vtxIdx : verts)
        {
            drawable.Attrib(mesh->vertexPositions[vtxIdx]);
            drawable.Attrib(mesh->vertexNormals[vtxIdx]);
            drawable.Attrib<byte>(255, 0, 0, 255);
        }
        drawable.Render();

        // clear the verts list because it's static to keep around the internal array
        verts.clear();
    }

    //-----------------------------------//

    GlMeshPtr App::BuildMesh(HalfEdgeMeshPtr mesh)
    {
        using namespace mesh;
        using namespace glmesh;

        Timer loadClock;
        loadClock.Reset();

        VertexFormat fmt{ *vtxFmt };

        CpuDataWriter writer{ fmt, mesh->triangles() * 3 };
        walk(faces(*mesh), [&](const FaceIterator& face) -> FaceIterator {

            auto fedges = edges(face);
            walk(begin(fedges), [&](const EdgeIterator& edge) -> EdgeIterator {
                writer.Attrib(edge.vtx(0));
                writer.Attrib(edge.normal(0));
                writer.Attrib<byte>(128, 128, 128, 255);

                return next(edge);
            });

            return next(face);
        });

        // send vtx data to gpu
        auto vtxBuffer = writer.TransferToBuffer(gl::ARRAY_BUFFER, gl::STATIC_DRAW);

        // create the attribute arrays
        std::array<uint32, 2> vao{ { 0, 0 } };
        gl::GenVertexArrays(2, &vao[0]);

        gl::BindBuffer(gl::ARRAY_BUFFER, vtxBuffer);

        // variant -> "all"
        gl::BindVertexArray(vao[0]);
        fmt.BindAttributes(0);

        // variant -> "pos-only"
        gl::BindVertexArray(vao[1]);
        fmt.BindAttribute(0, 0);

        gl::BindBuffer(gl::ARRAY_BUFFER, 0);

        MeshVariantMap variants;
        variants["all"] = vao[0];
        variants["pos-only"] = vao[1];

        // render commands
        RenderCmdList cmds;
        cmds.DrawArrays(gl::TRIANGLES, 0, mesh->triangles() * 3);

        auto newMesh = new Mesh{ { { vtxBuffer } }, vao[0], cmds, variants };
        GlMeshPtr ptr;
        ptr.reset(newMesh);

        // log it
        auto time = loadClock.GetElapsed();
        log("build time: %0s", time);

        return ptr;
    }

    //-----------------------------------//

    ShaderPtr App::LoadShader(const String& baseName)
    {
        static String msg;

        String vtx;
        String frag;

        // generate file names
        mini::format(vtx, "%0%1.vs.glsl", shaderBinPath.string().c_str(), baseName.c_str());
        mini::format(frag, "%0%1.fs.glsl", shaderBinPath.string().c_str(), baseName.c_str());

        // create human-friendly log message
        mini::format(msg, "loaded shader [\"%0\"] {\"%1\", \"%2\"} ", baseName.c_str(), vtx.c_str(), frag.c_str());

        // hash shader base-name & files
        std::array<uint64, 2> keys = { {
            MurmurHash64(vtx.c_str(), vtx.size(), 0)
            , MurmurHash64(frag.c_str(), frag.size(), 0) } };

        // load the text from file
        vtx = std::move(LoadTextFromFile(fs::path{ vtx }));
        frag = std::move(LoadTextFromFile(fs::path{ frag }));

        // store shader for future reference
        auto idx = CreateShader(vtx, frag);
        ShaderPtr prog;
        if (idx != 0)
        {
            prog = std::make_shared<Shader>(baseName, idx);

            shaders[baseName] = prog;
            loadedShaders.set(keys[0], baseName);
            loadedShaders.set(keys[1], baseName);

            log(msg.c_str());
        }

        return prog;
    }

    //-----------------------------------//

    uint32 App::CreateShader(const String& vtxShader, const String& fragShader)
    {
        // compile / link
        uint32 prog = 0;

        try
        {
            prog = glutil::LinkProgram(vtxShader, fragShader);
        }
        catch (const glutil::CompileLinkException& e)
        {
            log("failed to compile/link shader: %0", e.what());
            return 0;
        }

        return prog;
    }

    //-----------------------------------//

    String App::LoadTextFromFile(boost::filesystem::path file)
    {
        String text;

        // error handling
        if (!exists(file))
        {
            log("cannot open file \"%0\"", file.string().c_str());
            return text;
        }
        else if (is_directory(file))
        {
            log("cannot load directory as file \"%0\"", file.string().c_str());
            return text;
        }

        // load and format
        fs::ifstream stream{ file, std::ios::in };
        std::stringstream ss;

        ss << stream.rdbuf();
        text = ss.str();

        // notify the filewatcher
        send(core->root->events, "watch-file", file.string());

        // SEND IT HOME, SON
        return std::move(text);
    }

    //-----------------------------------//

    void App::OnFileWatch(WatchID watchid, const String& dir, const String& filename, Action action)
    {
        // static helper data
        static std::array<String, 3> actionNames{ { "ADDED", "DELETED", "MODIFIED" } };

        // format event message
        String msg;
        mini::format(msg, "file watch - %0 \"%1%2\"", actionNames[(uint32) action].c_str(), dir.c_str(), filename.c_str());

        // prevent double-dispatched events
        auto eventHash = MurmurHash64(msg.c_str(), msg.size(), 0);
        if (watchEvents.find(eventHash) != watchEvents.end())
            return;

        watchEvents.insert(eventHash);
        log(msg.c_str());

        // determine if this file needs to be reloaded
        if (action == Actions::Modified)
        {
            fs::path file{ dir + filename };
            if (file.extension() == ".glsl")
            {
                auto key = MurmurHash64(file.string().c_str(), file.string().size(), 0);
                auto base = loadedShaders.get(key, String{});
                if (!base.empty())
                    LoadShader(base);
            }
        }
    }

    //-----------------------------------//

#pragma endregion
}