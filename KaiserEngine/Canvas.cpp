#include "stdafx.h"
#include "Canvas.h"
#include "loadgl.h"
#include "EventListener.h"
#include "MainWindow.h"
#include "GLTexture.h"

class CanvasEventListener
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
    std::string sSource, sType;

    switch (source)
    {
    case GL_DEBUG_SOURCE_APPLICATION:
        sSource = "GL_DEBUG_SOURCE_APPLICATION";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        sSource = "GL_DEBUG_SOURCE_THIRD_PARTY";
        break;
    default:
        sSource = "";
        break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        sType = "GL_DEBUG_TYPE_ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        sType = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        sType = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        sType = "GL_DEBUG_TYPE_PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        sType = "GL_DEBUG_TYPE_PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_MARKER:
        sType = "GL_DEBUG_TYPE_MARKER";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        sType = "GL_DEBUG_TYPE_PUSH_GROUP";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        sType = "GL_DEBUG_TYPE_POP_GROUP";
        break;
    case GL_DEBUG_TYPE_OTHER:
        sType = "GL_DEBUG_TYPE_OTHER";
        break;
    default:
        sType = "";
        break;
    }

    std::stringstream ss;
    ss << std::hex << "GL CALLBACK: "
        << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
        << " source = 0x" << source << " " << sSource
        << ", type = 0x" << type << " " << sType
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
        throw std::runtime_error("NullPointerException: hWnd(HWND) is nullptr!");
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

    if (screenWidth <= 0) throw std::runtime_error("Invalid screen width!");
    if (screenHeight <= 0) throw std::runtime_error("Invalid screen height!");

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

    {
        GLProgram& program{ this->CreateProgram("default") };

        // setup shaders
        std::map<GLenum, std::string> shaders{
            { GL_VERTEX_SHADER, "vertex.shader" },
            { GL_FRAGMENT_SHADER, "fragment.shader" },
        };

        const std::vector<std::string> attributes{
            "in_position",          // 0
            "in_color",             // 1
            "in_texture_coord",     // 2
        };

        std::map<std::string, GLint> uniforms{
            { "matrix_projection", 0 },
            { "matrix_view", 0 },
            { "matrix_model", 0 },
        };

        program.Setup(shaders, &attributes, &uniforms);

        // make matrices
#pragma warning(push)
#pragma warning(disable: 4244) // suppress the warning about the following implicit conversion from int to _Ty(float)
        const mat4 mp = MakePerspectiveProjectionMatrix<float>(screenWidth, screenHeight, degrees2radians(60.0f), 100.0f, 0.1f);
        const mat4 mv = MakeViewMatrix<float>();
        const mat4 mm = MakeModelMatrix<float>();
#pragma warning(pop)

        GLProgram::UseProgram(program);

        program.LoadUniformMatrix(uniforms["matrix_projection"], mp);
        program.LoadUniformMatrix(uniforms["matrix_view"], mv);
        program.LoadUniformMatrix(uniforms["matrix_model"], mm);

        GLProgram::UseProgram();
    }
}

template <typename _Ty>
struct VertexData
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
    static void EnableVertexAttribArray();
    static void DisableVertexAttribArray();
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

template <>
void
VertexData<float>::
SetVertexAttribPointer()
{
    const GLenum type{ GL_FLOAT };
    const GLboolean normalized{ GL_FALSE };
    const GLsizei stride{ sizeof(VertexData<float>) };

    const static int PositionByteCount  = VertexData::PositionElementCount  * sizeof(VertexData::DataType);
    const static int ColorByteCount     = VertexData::ColorElementCount     * sizeof(VertexData::DataType);
    const static int TextureByteCount   = VertexData::TextureElementCount   * sizeof(VertexData::DataType);

    const static int PositionByteOffset = 0;
    const static int ColorByteOffset    = PositionByteOffset + PositionByteCount;
    const static int TextureByteOffset  = ColorByteOffset + ColorByteCount;

#pragma warning(push)
#pragma warning(disable: 4312)
    ::SetVertexAttribPointer(0,
        VertexData<float>::PositionElementCount,
        type, normalized, stride,
        PositionByteOffset);
    ::SetVertexAttribPointer(1,
        VertexData<float>::ColorElementCount,
        type, normalized, stride,
        reinterpret_cast<void*>(ColorByteOffset));
    ::SetVertexAttribPointer(2,
        VertexData<float>::TextureElementCount,
        type, normalized, stride,
        reinterpret_cast<void*>(TextureByteOffset));
#pragma warning(pop)
}

static
void
EnableVertexAttribArray(const GLuint& index)
{
    glEnableVertexAttribArray(index);
    {
        std::stringstream ss;
        ss << "glEnableVertexAttribArray("
            << index
            << ")"
            << std::endl;
        DetectGLError(ss);
    }
}

template <typename _Ty>
void
VertexData<_Ty>::
EnableVertexAttribArray()
{
    ::EnableVertexAttribArray(0);
    ::EnableVertexAttribArray(1);
    ::EnableVertexAttribArray(2);
}

template <typename _Ty>
void
VertexData<_Ty>::
DisableVertexAttribArray()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    SuppressGLError();
}

static
void
LoadTexture(GLTexture& texture, const std::wstring& path)
{
    const Image image{ Image::FromFile(path) };
    
    texture.Create();
    texture.ActiveTexture(GL_TEXTURE0);
    texture.SetTarget(GL_TEXTURE_2D);
    texture.Bind();
    texture.SetPixelStorage(GL_UNPACK_ALIGNMENT, 1);
    texture.SetImage(image, GL_TEXTURE_2D, 0, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE);
    texture.GenerateMipmap();

    // setup the ST coordinate system
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    {
        std::stringstream ss;
        ss << "glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)"
            << std::endl;
        DetectGLError(ss);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    {
        std::stringstream ss;
        ss << "glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)"
            << std::endl;
        DetectGLError(ss);
    }
    // setup what to do when the texture has to be scaled
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    {
        std::stringstream ss;
        ss << "glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)"
            << std::endl;
        DetectGLError(ss);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    {
        std::stringstream ss;
        ss << "glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)"
            << std::endl;
        DetectGLError(ss);
    }

    texture.Unbind();
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
    VertexData<float> vertexData[] = {
        VertexData<float>{
            -.5f, .5f, .0f, .0f,
            .0f, .0f, .0f, .0f,
            .0f, .0f,
        },
        VertexData<float>{
            -.5f, -.5f, .0f, .0f,
            .0f, .0f, .0f, .0f,
            .0f, 1.f,
        },
        VertexData<float>{
            .5f, -.5f, .0f, .0f,
            .0f, .0f, .0f, .0f,
            1.f, 1.f,
        },
        VertexData<float>{
            .5f, .5f, .0f, .0f,
            .0f, .0f, .0f, .0f,
            1.f, .0f,
        },
    };
    vbo.SetData(sizeof(vertexData), vertexData);
    VertexData<float>::SetVertexAttribPointer();
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

    // Setup textures
    auto path = L"test.bmp";
    GLTexture& texture{ this->CreateTexture("default") };
    LoadTexture(texture, path);
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

    // bind textures if any
    GLProgram& program{ this->GetProgram("default") };
    GLProgram::UseProgram(program);

    GLTexture& texture{ this->GetTexture("default") };
    texture.ActiveTexture(GL_TEXTURE0);
    texture.Bind();

    // bind VAOs if any
    GLVertexArray& vao{ this->GetVertexArray("quad") };
    vao.Bind();
    VertexData<float>::EnableVertexAttribArray();

    // bind VBOs if any
    GLBuffer& vboi{ vao.GetBuffer("index") };
    vboi.Bind();

    // draw vertices
    const GLsizeiptr& count{ vboi.GetCount() };
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count & 0xffffffff), GL_UNSIGNED_BYTE, 0);
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
    VertexData<float>::DisableVertexAttribArray();
    vao.Unbind();

    GLProgram::UseProgram();

    HDC hDC = wglGetCurrentDC();
    SwapBuffers(hDC);
}

void
Canvas::
Dispose()
{
    VertexData<float>::DisableVertexAttribArray();

    for (auto itr = vaos.begin();
        itr != vaos.end();
        itr++)
    {
        GLVertexArray& vao{ itr->second };
        vao.Destroy();
    }
    this->vaos.clear();

    for (auto itr = texs.begin();
        itr != texs.end();
        itr++)
    {
        GLTexture& texture{ itr->second };
        texture.Destroy();
    }
    this->texs.clear();

    for (auto& [name, program] : this->programs)
    {
        program.Destroy();
    }
    this->programs.clear();

    SuppressGLError();
}

GLProgram&
Canvas::
GetProgram(const std::string& name)
{
    return this->programs.at(name);
}

GLVertexArray&
Canvas::
GetVertexArray(const std::string& name)
{
    return this->vaos.at(name);
}

GLTexture&
Canvas::
GetTexture(const std::string& name)
{
    return this->texs.at(name);
}

GLVertexArray&
Canvas::
CreateVertexArray(const std::string& name)
{
    auto& [itr, result] = this->vaos.try_emplace(name, GLVertexArray());
    if (!result)
        throw std::runtime_error("Fail to emplace GLVertexArray with duplicate names!");
    itr->second.Create();
    return itr->second;
}

GLProgram&
Canvas::
CreateProgram(const std::string& name)
{
    auto& [itr, result] = this->programs.try_emplace(name, GLProgram());
    if (!result)
        throw std::runtime_error("Fail to emplace GLProgram with duplicate names!");
    itr->second.Create();
    return itr->second;
}

GLTexture&
Canvas::
CreateTexture(const std::string& name)
{
    auto& [itr, result] = this->texs.try_emplace(name, GLTexture());
    if (!result)
        throw std::runtime_error("Fail to emplace GLTexture with duplicate names!");
    itr->second.Create();
    return itr->second;
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
