#include "stdafx.h"
#include "Canvas.h"
#include "loadgl.h"
#include "EventListener.h"
#include "MainWindow.h"
#include "GLTexture.h"

class CanvasEventListener final
    : public WindowEventListener
{
private:
    Canvas& canvas;
public:
    CanvasEventListener(Canvas& canvas, const EventHandler& hdlr, HWND hWnd);
    ~CanvasEventListener();
    LRESULT OnResize(const int& newWidth, const int& newHeight) override;
    LRESULT OnTimer() override;
};

using namespace MatrixMath;
using mat4 = MatrixQ<float, 4>;

Canvas::
Canvas(const AbstractWindow& window)
    : parent{ window }
{
    if (const MainWindow* mainWin = dynamic_cast<const MainWindow*>(&window))
    {
        EventManager* manager = const_cast<EventManager*>(static_cast<const EventManager*>(mainWin));
        EventHandler* handler = static_cast<EventHandler*>(manager);
        LPEventListener& listeners = handler->GetListeners();
        HWND hWnd = window.GetWindowHandle();
        listeners.push_back(std::make_unique<CanvasEventListener>(*this, *manager, hWnd));
    }
}

Canvas::
~Canvas()
{
}

const AbstractWindow&
Canvas::
GetParent() const
{
    return parent;
}

// @see: https://www.scratchapixel.com/
// @see: http://www.songho.ca/opengl/gl_projectionmatrix.html
template <typename _Ty>
const MatrixQ<_Ty, 4>
MakePerspectiveProjectionMatrix(
    const _Ty& aspectRatio,
    const _Ty& fieldOfView,
    const _Ty& zFar,
    const _Ty& zNear)
{
    const _Ty yScale = 1 / std::tan(fieldOfView / 2);
    const _Ty xScale = yScale / aspectRatio;
    const _Ty negatvieFrustumLength = zNear - zFar;
    MatrixQ<_Ty, 4> mq4;
    mq4.setData(0, 0, xScale);
    mq4.setData(1, 1, yScale);
    mq4.setData(2, 2, (zFar + zNear) / negatvieFrustumLength);
    mq4.setData(2, 3, (2 * zFar * zNear) / negatvieFrustumLength);
    mq4.setData(3, 2, -1);

    return mq4;
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakePerspectiveProjectionMatrix(
    const _Ty& width, const _Ty& height,
    const _Ty& fieldOfView,
    const _Ty& zFar,
    const _Ty& zNear)
{
    const _Ty aspectRatio = width / height;
    return MakePerspectiveProjectionMatrix<_Ty>(aspectRatio, fieldOfView, zFar, zNear);
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeOrthographicProjectionMatrix(
    const _Ty& right,
    const _Ty& left,
    const _Ty& top,
    const _Ty& bottom,
    const _Ty& zFar,
    const _Ty& zNear)
{
    return MatrixQ<_Ty, 4>{
        2 / (right - left), 0, 0, 0,
        0, 2 / (top - bottom), 0, 0,
        0, 0, -2 / (zFar - zNear), 0,
        (right + left) / (left - right), (top + bottom) / (bottom - top), (zFar + zNear) / (zNear - zFar), 1,
    };
}

// simplified because:
// (1) right + left = 0
// (2) top + bottom = 0
template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeOrthographicProjectionMatrix(
    const _Ty& right,
    const _Ty& top,
    const _Ty& zFar,
    const _Ty& zNear)
{
    return MakeOrthographicProjectionMatrix(right, -right, top, -top, zFar, zNear);
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeViewMatrix()
{
    return IdentityMatrix<_Ty, 4>();
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeModelMatrix()
{
    return IdentityMatrix<_Ty, 4>();
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeTranslationMatrix(const _Ty& x, const _Ty& y, const _Ty& z)
{
    return MatrixQ<_Ty, 4>{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1,
    };
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeScalingMatrix(const _Ty& Sx, const _Ty& Sy, const _Ty& Sz)
{
    return MatrixQ<_Ty, 4>{
        Sx, 0, 0, 0,
        0, Sy, 0, 0,
        0, 0, Sz, 0,
        0, 0, 0, 1,
    };
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeRotationMatrix_x(const _Ty& Rx)
{
    const _Ty cos = std::cos(Rx);
    const _Ty sin = std::sin(Rx);
    return MatrixQ<_Ty, 4>{
        1, 0, 0, 0,
        0, cos, -sin, 0,
        0, sin, cos, 0,
        0, 0, 0, 1,
    };
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeRotationMatrix_y(const _Ty& Ry)
{
    const _Ty cos = std::cos(Ry);
    const _Ty sin = std::sin(Ry);
    return MatrixQ<_Ty, 4>{
        cos, 0, sin, 0,
        0, 1, 0, 0,
        -sin, 0, cos, 0,
        0, 0, 0, 1,
    };
}

template <typename _Ty>
const MatrixQ<_Ty, 4>
MakeRotationMatrix_z(const _Ty& Rz)
{
    const _Ty cos = std::cos(Rz);
    const _Ty sin = std::sin(Rz);
    return MatrixQ<_Ty, 4>{
        cos, -sin, 0, 0,
        sin, cos, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
    };
}

template <typename _Ty>
const Vector<_Ty, 4>
Transform(const MatrixQ<_Ty, 4>& TranslationMatrix,
    const MatrixQ<_Ty, 4>& RotationMatrix,
    const MatrixQ<_Ty, 4>& ScaleMatrix,
    const Vector<_Ty, 4>& OriginalVector)
{
    const Vector<_Ty, 4> TransformedVector = TranslationMatrix * RotationMatrix * ScaleMatrix * OriginalVector;
    return TransformedVector;
}

static
void
CALLBACK
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::stringstream ss;
    ss << std::hex << "GL CALLBACK: "
        << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
        << " source = 0x" << source
        << ", type = 0x" << type
        << ", id = 0x" << id
        << ", severity = 0x" << severity
        << ", message = " << message
        << std::endl;
    OutputDebugStringA(ss.str().c_str());
}


void
Canvas::
Setup()
{
    RECT rect = { 0 };
    const AbstractWindow& parent = this->GetParent();
    HWND hWnd = parent.GetWindowHandle();
    if (hWnd == nullptr)
        throw std::exception("NullPointerException: hWnd(HWND) is nullptr!");
    GetClientRect(hWnd, &rect);
    const LONG screenWidth = rect.right - rect.left;
    const LONG screenHeight = rect.bottom - rect.top;
    this->Setup(screenWidth, screenHeight);
}

void
Canvas::
Setup(const int& screenWidth, const int& screenHeight)
{
    static const int trimX = 10;
    static const int trimY = 10;

    if (screenWidth <= 0) throw std::exception("Invalid screen width!");
    if (screenHeight <= 0) throw std::exception("Invalid screen height!");

    const GLsizei width{ screenWidth - 2 * trimX };
    const GLsizei height{ screenHeight - 2 * trimY };
    glViewport(trimX, trimY, width, height);
    {
        std::stringstream ss;
        ss << "glViewport(" << trimX << ", "
            << trimY << ", "
            << width << ", "
            << height << ")";
        DetectGLError(ss.str().c_str());
    }

    if (programs.find("default") != programs.end()) // no program is named as default
    {
        // setup shaders
        std::map<GLenum, GLstring> shaders;
        shaders[GL_VERTEX_SHADER] = "vertex.shader";
        shaders[GL_FRAGMENT_SHADER] = "fragment.shader";

        std::vector<GLstring> attributes{
            "in_position",          // 0
            "in_color",             // 1
            "in_texture_coord",     // 2
        };

        std::map<GLstring, GLint> uniforms;
        uniforms["matrix_projection"] = 0;
        uniforms["matrix_view"] = 0;
        uniforms["matrix_model"] = 0;


        GLProgram program;
        programs["default"] = program;
        program.Setup(shaders, &attributes, &uniforms);

        // beware the following matrices are instances of MatrixQ<double, 4>

        // make matrices
#pragma warning(push)
#pragma warning(disable: 4244) // suppress the warning about the following implicit conversion from int to _Ty(float)
        const mat4 mp = MakePerspectiveProjectionMatrix<float>(screenWidth, screenHeight, degrees2radians(60.0f), 100.0f, 0.1f);
        const mat4 mv = MakeViewMatrix<float>();
        const mat4 mm = MakeModelMatrix<float>();
#pragma warning(pop)

        GLProgram::UseProgram(&program);

        program.LoadUniformMatrix(uniforms["matrix_projection"], mp);
        program.LoadUniformMatrix(uniforms["matrix_view"], mv);
        program.LoadUniformMatrix(uniforms["matrix_model"], mm);

        GLProgram::UseProgram();
    }
}

template <typename _Ty>
struct VertexData final
{
    struct      // positions
    {
        _Ty x;
        _Ty y;
        _Ty z;
        _Ty w;
    };
    struct      // colors
    {
        _Ty r;
        _Ty g;
        _Ty b;
        _Ty a;
    };
    struct      // texture coordinates
    {
        _Ty u;
        _Ty v;
    };

    using DataType = _Ty;

    VertexData();
    VertexData(const std::initializer_list<_Ty>& initializerList);
    ~VertexData() {}

    const static int PositionElementCount = 4;      /* x, y, z, w */
    const static int ColorElementCount = 4;         /* r, g, b, a */
    const static int TextureElementCount = 2;       /* u, v */

    static void SetVertexAttribPointer();
};

template <typename _Ty>
VertexData<_Ty>::
VertexData()
    : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 }
    , r{ 0 }, g{ 0 }, b{ 0 }, a{ 0 }
    , u{ 0 }, v{ 0 }
{
}

template<>
VertexData<float>::
VertexData()
    : x{ 0.0f }, y{ 0.0f }, z{ 0.0f }, w{ 0.0f }
    , r{ 0.0f }, g{ 0.0f }, b{ 0.0f }, a{ 0.0f }
    , u{ 0.0f }, v{ 0.0f }
{
}

template <typename _Ty>
VertexData<_Ty>::
VertexData(const std::initializer_list<_Ty>& initializerList)
    : VertexData()
{
    _Ty* dataBegin = &(this->x);
    std::copy(initializerList.begin(), initializerList.end(), dataBegin);
}

template <typename _Ty>
void
VertexData<_Ty>::
SetVertexAttribPointer()
{
    throw std::runtime_error("Unsupported <typename _Ty>!");
}

static
void
SetVertexAttribPointer(
    const GLuint& index,
    const GLint& size,
    const GLenum& type,
    const GLboolean& normalized,
    const GLsizei& stride,
    const void* pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    {
        std::stringstream ss;
        ss << "glVertexAttribPointer("
            << index << ", "
            << size << ", "
            << std::hex << type << std::dec << ", "
            << std::boolalpha << normalized << std::noboolalpha << ", "
            << stride << ", "
            << pointer
            << ")"
            << std::endl;
        DetectGLError(ss);
    }
}

// @see: http://falloutsoftware.com/tutorials/gl/gl2.htm
void
Canvas::
Prepare()
{
    glEnable(GL_DEBUG_OUTPUT);
    DetectGLError("glEnable");
    glDebugMessageCallback(MessageCallback, nullptr);
    DetectGLError("glDebugMessageCallback");

    // Setup an XNA like background color
    glClearColor(0.4f, 0.6f, 0.9f, 0.0f);
    DetectGLError("glClearColor");

    this->Setup();
    // prepare opengl objects

    GLVertexArray& vao{ this->CreateVertexArray("quad") };
    vao.Create();
    vao.Bind();

    GLBuffer& vbo{ vao.CreateBuffer("vertex") };
    vbo.Create();
    vbo.SetTarget(GL_ARRAY_BUFFER);
    vbo.SetUsage(GL_STATIC_DRAW);
    vbo.Bind();
    float vertices[] =
    {
        -.5f, .5f, .0f,
        -.5f, -.5f, .0f,
        .5f, -.5f, .0f,
        .5f, .5f, .0f,
    };
    vbo.SetData(sizeof(vertices), vertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
    {
        std::stringstream ss;
        ss << "glVertexAttribPointer(...)"
            << std::endl;
        DetectGLError(ss);
    }
    vbo.Unbind();

    vao.Unbind();

    char indices[] =
    {
        0, 1, 2,
        2, 3, 0,
    };
    GLBuffer& vboi{ vao.CreateBuffer("index") };
    vboi.Create();
    vboi.SetTarget(GL_ELEMENT_ARRAY_BUFFER);
    vboi.SetUsage(GL_STATIC_DRAW);
    vboi.Bind();
    vboi.SetData(sizeof(indices), indices);
    vboi.Unbind();
}

void
Canvas::
Clear()
{
    glClear(GL_COLOR_BUFFER_BIT
        | GL_DEPTH_BUFFER_BIT);
    DetectGLError("glClear");
}

void
Canvas::
Render()
{
    this->Clear();

    // TODO
    //OutputDebugStringA("Rendering ... \r\n");

    // bind textures if any

    // bind VAOs if any

    // bind VBOs if any
    GLVertexArray& vao{ this->GetVertexArray("quad") };
    vao.Bind();
    glEnableVertexAttribArray(0);
    {
        std::stringstream ss;
        ss << "glEnableVertexAttribArray(0)"
            << std::endl;
        DetectGLError(ss);
    }

    GLBuffer& vboi{ vao.GetBuffer("index") };
    vboi.Bind();

    // draw vertices
    glDrawElements(GL_TRIANGLES, vboi.GetCount(), GL_UNSIGNED_BYTE, 0);
    {
        std::stringstream ss;
        ss << "glDrawElements(GL_TRIANGLES, "
            << vboi.GetCount()
            << ", GL_UNSIGNED_BYTE, 0)"
            << std::endl;
        DetectGLError(ss);
    }

    vboi.Unbind();

    // put everything back to default
    glDisableVertexAttribArray(0);
    SuppressGLError();
    vao.Unbind();

    HDC hDC = wglGetCurrentDC();
    SwapBuffers(hDC);
}

void
Canvas::
Dispose()
{
    glDisableVertexAttribArray(0);
    SuppressGLError();

    for (auto itr = vaos.begin();
        itr != vaos.end();
        itr++)
    {
        GLVertexArray& vao{ itr->second };
        vao.Destroy();
    }
    this->vaos.clear();

    SuppressGLError();
}

bool
Canvas::
AddVertexArray(GLstring name, const GLVertexArray& array)
{
    if (this->vaos.find(name) != this->vaos.end())
        return false;

    this->vaos[name] = array;
    return true;
}

GLVertexArray&
Canvas::
GetVertexArray(GLstring name)
{
    return this->vaos.at(name);
}

GLVertexArray&
Canvas::
CreateVertexArray(GLstring name)
{
    this->AddVertexArray(name, GLVertexArray());
    return this->GetVertexArray(name);
}

// CanvasEventListener

CanvasEventListener::
CanvasEventListener(Canvas& canvas, const EventHandler& hdlr, HWND hWnd)
    : WindowEventListener(hdlr, hWnd)
    , canvas{ canvas }
{
}

CanvasEventListener::
~CanvasEventListener()
{
}

LRESULT
CanvasEventListener::
OnResize(const int& newWidth, const int& newHeight)
{
    this->canvas.Setup(newWidth, newHeight);
    return 0;
}

LRESULT
CanvasEventListener::
OnTimer()
{
    this->canvas.Render();
    return 0;
}
