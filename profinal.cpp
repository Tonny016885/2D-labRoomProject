#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
//function declare 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void drawShape(unsigned int shaderProgram, unsigned int VAO, unsigned int VBO,
    float* verts, int count, float r, float g, float b, float alpha = 1.0f);
void drawQuad(unsigned int sp, unsigned int VAO, unsigned int VBO,
    float x1, float y1, float x2, float y2,
    float r, float g, float b, float alpha = 1.0f);

//window setting korsi ei part e 
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 860;

//2d house  design er motoi ei shadderta rakhsi eitate kono change nai
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";

//fragment shadder e uniform color disi jekhane ami baire theke color pathacchi shadder e & each shape draw korar age alada color send korte parbo
const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 uColor;\n"          // agey eikhane vec3 use korchi ekhon, vec4 (R,G,B,A) eikhe A mane hocche transpericy
"void main()\n"
"{\n"
"   FragColor = uColor;\n"
"}\n\0";

//color er naam gulaa pore read korar jonno
// Room er structure
#define COL_WALL_BG      0.18f, 0.20f, 0.26f          // dark blue-gray deyaal
#define COL_FLOOR        0.78f, 0.68f, 0.54f          // halkaa badami color er floor
#define COL_CEILING      0.22f, 0.24f, 0.30f          // halkaa color er ceiing
#define COL_FLOOR_LINE   0.65f, 0.56f, 0.42f          // floor er tile lines

// Blackboard er vertices 
#define COL_BOARD_FRAME  0.35f, 0.20f, 0.08f          //garo mehegoni rong
#define COL_BOARD        0.09f, 0.28f, 0.15f          // garo green board
#define COL_BOARD_TRAY   0.28f, 0.16f, 0.06f          // chalk rakhar tray
#define COL_CHALK        0.95f, 0.95f, 0.92f          // chalk er shada daag er jonno

// Furniture er structure
#define COL_TABLE_TOP    0.55f, 0.35f, 0.14f          // mjhari oak
#define COL_TABLE_DARK   0.38f, 0.22f, 0.07f          // gaaroo oak legs
#define COL_TABLE_SHINE  0.65f, 0.45f, 0.22f          // highlight kora hocche table er upor 
#define COL_CHAIR_SEAT   0.22f, 0.28f, 0.40f          // steel blue colorer seat
#define COL_CHAIR_FRAME  0.18f, 0.18f, 0.22f          // dark metal frame
#define COL_CHAIR_PAD    0.28f, 0.36f, 0.52f          // seat cushion k highlight er jonno
// PC ba monitor er jonno
#define COL_MONITOR_BODY 0.14f, 0.14f, 0.18f          // near-black plastic rakhchi
#define COL_MONITOR_EDGE 0.22f, 0.22f, 0.28f          // bezel kinaraa
#define COL_SCREEN_OFF   0.06f, 0.08f, 0.12f          // screen off obosthay rakha hoise
#define COL_SCREEN_ON    0.10f, 0.48f, 0.82f          // screen glow blue
#define COL_SCREEN_GLOW  0.35f, 0.72f, 1.00f          // screen highlight kora hoise ekhane
#define COL_STAND        0.20f, 0.20f, 0.24f          // monitor stand er jonno

// Door & Window
#define COL_DOOR         0.50f, 0.30f, 0.10f          // walnut door
#define COL_DOOR_PANEL   0.44f, 0.26f, 0.08f          // recessed panel
#define COL_DOORKNOB     0.85f, 0.72f, 0.20f          // brass knob
#define COL_WIN_FRAME    0.42f, 0.26f, 0.10f          // window frame wood
#define COL_WIN_GLASS    0.55f, 0.80f, 0.92f          // glass sky blue
#define COL_WIN_SHINE    0.80f, 0.95f, 1.00f          // glass reflection

// Lighting / Ceiling lamp
#define COL_LAMP_BASE    0.70f, 0.70f, 0.72f          // aluminum lamp
#define COL_LAMP_GLOW    0.99f, 0.96f, 0.80f          // warm light

// Teacher zone
#define COL_TEACHER_DESK 0.48f, 0.28f, 0.10f          // darker wood
#define COL_PODIUM       0.40f, 0.24f, 0.08f          // podium

int main()
{
    //eikhane glfw initialize kora hose ager 2d er house code er motoei part er moddhe change kora hoy nai
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
        "  2D Computer Lab  |  Sumiya Akter  |  ID: 41230201045", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    //shadder comple kore link kora hoise agr 2d house er code er motoi eitte kono chage kor ni 
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShaderSource, NULL);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShaderSource, NULL);
    glCompileShader(fs);

    unsigned int sp = glCreateProgram();
    glAttachShader(sp, vs);
    glAttachShader(sp, fs);
    glLinkProgram(sp);
    glDeleteShader(vs);
    glDeleteShader(fs);

    //eikhane bufferr change korsi karon lab room e approximate 150+ shape draw korsi tai bigger buffer lagbe eijonno 2048 t nisi
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 2048 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    //ekhame new kore alpha blnding enable korsi karon alpha blending chara tranperecy kaaj korbe na kron amra vec4 use korsi
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //eikhan theke render loop strt hoise
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.12f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(sp);

        //eita room shell er part
 //piichoner wall er full backgrund
        drawQuad(sp, VAO, VBO, -1.0f, 0.05f, 1.0f, 1.0f, COL_WALL_BG);

        // Floor er 3 ta stripes er gradient perspective nisi
        drawQuad(sp, VAO, VBO, -1.0f, -1.0f, 1.0f, -0.40f, 0.82f, 0.72f, 0.57f);
        drawQuad(sp, VAO, VBO, -1.0f, -0.40f, 1.0f, -0.10f, 0.75f, 0.64f, 0.50f);
        drawQuad(sp, VAO, VBO, -1.0f, -0.10f, 1.0f, 0.05f, 0.68f, 0.56f, 0.42f);

        // horizontally floor er oddhe tiles line boshaisi
        for (int i = 0; i < 4; i++) {
            float fy = -0.98f + i * 0.28f;
            drawQuad(sp, VAO, VBO, -1.0f, fy, 1.0f, fy + 0.012f, COL_FLOOR_LINE);
        }
        // Floor tile lines vertically boshaisi
        for (int i = 0; i < 7; i++) {
            float fx = -0.90f + i * 0.30f;
            drawQuad(sp, VAO, VBO, fx, -1.0f, fx + 0.012f, 0.05f, COL_FLOOR_LINE);
        }

        // Ceiling er part
        drawQuad(sp, VAO, VBO, -1.0f, 0.88f, 1.0f, 1.0f, COL_CEILING);

        // Wallfloor er dark border line part
        drawQuad(sp, VAO, VBO, -1.0f, 0.02f, 1.0f, 0.06f, 0.12f, 0.12f, 0.16f);

        // Left wall er sider strip
        drawQuad(sp, VAO, VBO, -1.0f, -1.0f, -0.96f, 1.0f, 0.14f, 0.16f, 0.20f);
        // Right wall er side er trip
        drawQuad(sp, VAO, VBO, 0.96f, -1.0f, 1.0f, 1.0f, 0.14f, 0.16f, 0.20f);

        //3taa celieng lamp er part eitaa
        float lampX[] = { -0.60f, 0.0f, 0.60f };
        for (int i = 0; i < 3; i++) {
            float lx = lampX[i];
            // Lamp er taar er part
            drawQuad(sp, VAO, VBO, lx - 0.008f, 0.88f, lx + 0.008f, 0.95f, 0.50f, 0.50f, 0.52f);
            // Lamp er shade er part
            float shadeVerts[] = {
                lx - 0.07f, 0.82f, 0.0f,
                lx + 0.07f, 0.82f, 0.0f,
                lx + 0.04f, 0.88f, 0.0f,
                lx - 0.07f, 0.82f, 0.0f,
                lx + 0.04f, 0.88f, 0.0f,
                lx - 0.04f, 0.88f, 0.0f,
            };
            drawShape(sp, VAO, VBO, shadeVerts, 6, COL_LAMP_BASE);
            // Light glow hocche odhek transparent bhabe
            drawQuad(sp, VAO, VBO, lx - 0.06f, 0.78f, lx + 0.06f, 0.82f, COL_LAMP_GLOW, 0.90f);
            // alo ceilin e choray portese oitar part
            drawQuad(sp, VAO, VBO, lx - 0.12f, 0.82f, lx + 0.12f, 0.88f, 0.99f, 0.95f, 0.75f, 0.25f);
        }

        //mjhkhane blackboard er part 
         // Outer frame er shadow er part 
        drawQuad(sp, VAO, VBO, -0.62f, 0.44f, 0.62f, 0.88f, 0.08f, 0.08f, 0.10f);
        // Mahogany frame er prt
        drawQuad(sp, VAO, VBO, -0.60f, 0.46f, 0.60f, 0.87f, COL_BOARD_FRAME);
        // Green board surface er part
        drawQuad(sp, VAO, VBO, -0.57f, 0.49f, 0.57f, 0.84f, COL_BOARD);
        // Board er bhitore ektu highlight er part
        drawQuad(sp, VAO, VBO, -0.57f, 0.81f, 0.57f, 0.84f, 0.12f, 0.36f, 0.20f);
        // Chalk tray er jonno
        drawQuad(sp, VAO, VBO, -0.57f, 0.46f, 0.57f, 0.50f, COL_BOARD_TRAY);

        //chalk er white lne gular jonno(eitar further working korar scope ase)
        float chalkMarks[][4] = {
            {-0.52f,0.65f,-0.44f,0.67f}, {-0.42f,0.65f,-0.34f,0.67f},
            {-0.32f,0.65f,-0.24f,0.67f}, {-0.22f,0.65f,-0.14f,0.67f},
            {-0.12f,0.65f,-0.04f,0.67f}, { 0.04f,0.65f, 0.12f,0.67f},
            { 0.14f,0.65f, 0.22f,0.67f}, { 0.24f,0.65f, 0.32f,0.67f},
        };
        for (auto& m : chalkMarks)
            drawQuad(sp, VAO, VBO, m[0], m[1], m[2], m[3], COL_CHALK, 0.80f);

        // Horizontal chalk line jeita ase lekhar nche oitar part
        drawQuad(sp, VAO, VBO, -0.53f, 0.62f, 0.53f, 0.633f, COL_CHALK, 0.40f);

        // Chalk pieces try er upor ase oitar jonno
        drawQuad(sp, VAO, VBO, -0.35f, 0.463f, -0.28f, 0.477f, 0.95f, 0.95f, 0.90f);
        drawQuad(sp, VAO, VBO, -0.20f, 0.463f, -0.14f, 0.477f, 0.95f, 0.82f, 0.70f);
        drawQuad(sp, VAO, VBO, 0.10f, 0.463f, 0.17f, 0.477f, 0.95f, 0.95f, 0.90f);

        //teaher er jayga

        // Podium base
        drawQuad(sp, VAO, VBO, -0.15f, 0.06f, 0.15f, 0.26f, COL_PODIUM);
        drawQuad(sp, VAO, VBO, -0.13f, 0.08f, 0.13f, 0.24f, 0.45f, 0.28f, 0.10f);

        // Teacher desk top 
        drawQuad(sp, VAO, VBO, -0.42f, 0.22f, 0.42f, 0.32f, COL_TEACHER_DESK);
        // Desk shine strip
        drawQuad(sp, VAO, VBO, -0.42f, 0.29f, 0.42f, 0.32f, COL_TABLE_SHINE, 0.35f);
        // Desk front face
        drawQuad(sp, VAO, VBO, -0.42f, 0.06f, 0.42f, 0.22f, COL_TABLE_DARK);
        // Desk legs
        drawQuad(sp, VAO, VBO, -0.40f, 0.06f, -0.34f, 0.22f, 0.28f, 0.16f, 0.05f);
        drawQuad(sp, VAO, VBO, 0.34f, 0.06f, 0.40f, 0.22f, 0.28f, 0.16f, 0.05f);

        // Teacher monitor boro ta 
        drawQuad(sp, VAO, VBO, -0.09f, 0.32f, 0.09f, 0.44f, COL_MONITOR_BODY);
        drawQuad(sp, VAO, VBO, -0.085f, 0.325f, 0.085f, 0.435f, COL_MONITOR_EDGE);
        drawQuad(sp, VAO, VBO, -0.075f, 0.332f, 0.075f, 0.428f, COL_SCREEN_ON);
        // Screen glow highlight
        drawQuad(sp, VAO, VBO, -0.075f, 0.410f, 0.075f, 0.428f, COL_SCREEN_GLOW, 0.50f);
        drawQuad(sp, VAO, VBO, -0.075f, 0.332f, -0.020f, 0.428f, COL_SCREEN_GLOW, 0.12f);
        // Monitor stand
        drawQuad(sp, VAO, VBO, -0.012f, 0.28f, 0.012f, 0.32f, COL_STAND);
        drawQuad(sp, VAO, VBO, -0.028f, 0.274f, 0.028f, 0.285f, COL_STAND);

        // Keyboard on teacher desk
        drawQuad(sp, VAO, VBO, -0.09f, 0.225f, 0.09f, 0.245f, 0.20f, 0.20f, 0.24f);
        drawQuad(sp, VAO, VBO, -0.088f, 0.227f, 0.088f, 0.243f, 0.28f, 0.28f, 0.32f);

        //eita student desk eikhane 3 ta row o 4 ta coloumn er loop chalaisi
        float rowY[] = { -0.18f, -0.50f, -0.82f };
        float colX[] = { -0.76f, -0.30f,  0.16f,  0.62f };

        for (int row = 0; row < 3; row++)
        {
            float ty = rowY[row];          // table top Y
            float tb = ty - 0.065f;        // table bottom Y
            float legB = tb - 0.09f;         // leg bottom Y
            float cSeat = tb - 0.09f;       // chair seat top
            float cBot = cSeat - 0.06f;    // chair seat bottom

            for (int col = 0; col < 4; col++)
            {
                float cx = colX[col];
                float hw = 0.165f;  // half desk width

                // Shadow under desk
                drawQuad(sp, VAO, VBO, cx - hw + 0.01f, legB - 0.015f,
                    cx + hw + 0.01f, legB, 0.05f, 0.05f, 0.07f, 0.50f);

                // ---- TABLE ----
                // Table top surface
                drawQuad(sp, VAO, VBO, cx - hw, tb, cx + hw, ty, COL_TABLE_TOP);
                // Shine strip on table edge
                drawQuad(sp, VAO, VBO, cx - hw, ty - 0.012f, cx + hw, ty, COL_TABLE_SHINE, 0.45f);
                // Table front face (thickness)
                drawQuad(sp, VAO, VBO, cx - hw, tb - 0.015f, cx + hw, tb, COL_TABLE_DARK);
                // Left leg
                drawQuad(sp, VAO, VBO, cx - hw + 0.008f, legB, cx - hw + 0.024f, tb, COL_TABLE_DARK);
                // Right leg
                drawQuad(sp, VAO, VBO, cx + hw - 0.024f, legB, cx + hw - 0.008f, tb, COL_TABLE_DARK);
                // Leg crossbar
                drawQuad(sp, VAO, VBO, cx - hw + 0.008f, legB + 0.03f,
                    cx + hw - 0.008f, legB + 0.045f, COL_TABLE_DARK);

                //chair er part 
                float sw = 0.095f;  // seat half-width
                // Chair shadow
                drawQuad(sp, VAO, VBO, cx - sw + 0.01f, cBot - 0.01f,
                    cx + sw + 0.01f, cBot, 0.05f, 0.05f, 0.07f, 0.40f);
                // Seat cushion
                drawQuad(sp, VAO, VBO, cx - sw, cBot, cx + sw, cSeat, COL_CHAIR_SEAT);
                drawQuad(sp, VAO, VBO, cx - sw, cSeat - 0.012f, cx + sw, cSeat, COL_CHAIR_PAD, 0.60f);
                // Chair back uprights (2 posts)
                drawQuad(sp, VAO, VBO, cx - sw, cSeat, cx - sw + 0.018f, cSeat + 0.10f, COL_CHAIR_FRAME);
                drawQuad(sp, VAO, VBO, cx + sw - 0.018f, cSeat, cx + sw, cSeat + 0.10f, COL_CHAIR_FRAME);
                // Chair back top rail
                drawQuad(sp, VAO, VBO, cx - sw, cSeat + 0.082f, cx + sw, cSeat + 0.10f, COL_CHAIR_FRAME);
                // Chair back mid rail
                drawQuad(sp, VAO, VBO, cx - sw, cSeat + 0.040f, cx + sw, cSeat + 0.055f, COL_CHAIR_FRAME);
                // Chair legs (4 small stubs)
                float clx[] = { cx - sw + 0.01f, cx + sw - 0.026f };
                for (float lx2 : clx) {
                    drawQuad(sp, VAO, VBO, lx2, cBot - 0.045f, lx2 + 0.016f, cBot, COL_CHAIR_FRAME);
                }

                // mnitor er prt 
                float mw = 0.052f;
                float mBase = ty;
                // Monitor body
                drawQuad(sp, VAO, VBO, cx - mw, mBase, cx + mw, mBase + 0.095f, COL_MONITOR_BODY);
                // Bezel
                drawQuad(sp, VAO, VBO, cx - mw + 0.005f, mBase + 0.005f,
                    cx + mw - 0.005f, mBase + 0.090f, COL_MONITOR_EDGE);
                // Screen
                drawQuad(sp, VAO, VBO, cx - mw + 0.010f, mBase + 0.012f,
                    cx + mw - 0.010f, mBase + 0.083f, COL_SCREEN_ON);
                // Screen top glow
                drawQuad(sp, VAO, VBO, cx - mw + 0.010f, mBase + 0.068f,
                    cx + mw - 0.010f, mBase + 0.083f, COL_SCREEN_GLOW, 0.45f);
                // Screen left shine
                drawQuad(sp, VAO, VBO, cx - mw + 0.010f, mBase + 0.012f,
                    cx - mw + 0.022f, mBase + 0.083f, COL_SCREEN_GLOW, 0.10f);
                // Monitor stand
                drawQuad(sp, VAO, VBO, cx - 0.010f, mBase - 0.032f, cx + 0.010f, mBase, COL_STAND);
                drawQuad(sp, VAO, VBO, cx - 0.024f, mBase - 0.038f, cx + 0.024f, mBase - 0.025f, COL_STAND);
                // Keyboard
                drawQuad(sp, VAO, VBO, cx - mw + 0.005f, tb + 0.002f,
                    cx + mw - 0.005f, tb + 0.016f, 0.20f, 0.20f, 0.24f);
            }
        }

        //aro details add kore daan side e ekta dorj add korsi (further aro valo bhabe korr scope ase)
         // Door frame
        drawQuad(sp, VAO, VBO, 0.70f, -1.0f, 0.94f, 0.06f, 0.30f, 0.18f, 0.06f);
        // Door surface
        drawQuad(sp, VAO, VBO, 0.72f, -0.98f, 0.92f, 0.04f, COL_DOOR);
        // Upore panel
        drawQuad(sp, VAO, VBO, 0.74f, -0.10f, 0.90f, 0.02f, COL_DOOR_PANEL);
        drawQuad(sp, VAO, VBO, 0.75f, -0.09f, 0.89f, 0.01f, 0.54f, 0.33f, 0.12f);
        // nicher  panel
        drawQuad(sp, VAO, VBO, 0.74f, -0.70f, 0.90f, -0.14f, COL_DOOR_PANEL);
        drawQuad(sp, VAO, VBO, 0.75f, -0.69f, 0.89f, -0.15f, 0.54f, 0.33f, 0.12f);
        // Bottom panel
        drawQuad(sp, VAO, VBO, 0.74f, -0.97f, 0.90f, -0.74f, COL_DOOR_PANEL);
        drawQuad(sp, VAO, VBO, 0.75f, -0.96f, 0.89f, -0.75f, 0.54f, 0.33f, 0.12f);
        // Door knob plate
        drawQuad(sp, VAO, VBO, 0.732f, -0.44f, 0.752f, -0.36f, 0.65f, 0.55f, 0.15f);
        // Door knob
        drawQuad(sp, VAO, VBO, 0.734f, -0.415f, 0.750f, -0.385f, COL_DOORKNOB);

        //glss efect diye baam side e dorja
        // Outer frame shadow
        drawQuad(sp, VAO, VBO, -0.96f, 0.08f, -0.64f, 0.46f, 0.08f, 0.08f, 0.10f);
        // Window wood frame
        drawQuad(sp, VAO, VBO, -0.94f, 0.10f, -0.66f, 0.44f, COL_WIN_FRAME);
        // Glass pane
        drawQuad(sp, VAO, VBO, -0.92f, 0.12f, -0.68f, 0.42f, COL_WIN_GLASS, 0.75f);
        // Horizontal divider
        drawQuad(sp, VAO, VBO, -0.92f, 0.265f, -0.68f, 0.280f, COL_WIN_FRAME);
        // Vertical divider
        drawQuad(sp, VAO, VBO, -0.806f, 0.12f, -0.794f, 0.42f, COL_WIN_FRAME);
        // Glass reflection (diagonal shine)
        float glassShine[] = {
            -0.92f,0.38f,0.0f,
            -0.80f,0.42f,0.0f,
            -0.76f,0.42f,0.0f,
            -0.92f,0.38f,0.0f,
            -0.76f,0.42f,0.0f,
            -0.88f,0.34f,0.0f,
        };
        drawShape(sp, VAO, VBO, glassShine, 6, COL_WIN_SHINE, 0.30f);

        // janalar bairer view nel akash 
        drawQuad(sp, VAO, VBO, -0.916f, 0.124f, -0.800f, 0.262f, 0.45f, 0.72f, 0.92f, 0.60f);
        drawQuad(sp, VAO, VBO, -0.796f, 0.124f, -0.684f, 0.262f, 0.55f, 0.80f, 0.95f, 0.60f);
        drawQuad(sp, VAO, VBO, -0.916f, 0.283f, -0.800f, 0.416f, 0.40f, 0.65f, 0.85f, 0.60f);
        drawQuad(sp, VAO, VBO, -0.796f, 0.283f, -0.684f, 0.416f, 0.50f, 0.75f, 0.90f, 0.60f);

        //daan side er dwal ghori
        float clkX = 0.80f, clkY = 0.68f, clkR = 0.055f;
        // Clock face
        float clkFace[] = {
            clkX - clkR, clkY - clkR * 0.6f, 0.0f,
            clkX + clkR, clkY - clkR * 0.6f, 0.0f,
            clkX + clkR, clkY + clkR * 0.6f, 0.0f,
            clkX - clkR, clkY - clkR * 0.6f, 0.0f,
            clkX + clkR, clkY + clkR * 0.6f, 0.0f,
            clkX - clkR, clkY + clkR * 0.6f, 0.0f,
        };
        drawShape(sp, VAO, VBO, clkFace, 6, 0.92f, 0.90f, 0.85f);
        // Clock border
        float clkBorder[] = {
            clkX - clkR - 0.008f, clkY - clkR * 0.65f, 0.0f,
            clkX + clkR + 0.008f, clkY - clkR * 0.65f, 0.0f,
            clkX + clkR + 0.008f, clkY + clkR * 0.65f, 0.0f,
            clkX - clkR - 0.008f, clkY - clkR * 0.65f, 0.0f,
            clkX + clkR + 0.008f, clkY + clkR * 0.65f, 0.0f,
            clkX - clkR - 0.008f, clkY + clkR * 0.65f, 0.0f,
        };
        drawShape(sp, VAO, VBO, clkBorder, 6, 0.25f, 0.15f, 0.05f);
        // Redraw face on top
        drawShape(sp, VAO, VBO, clkFace, 6, 0.92f, 0.90f, 0.85f);
        // Clock hands
        drawQuad(sp, VAO, VBO, clkX - 0.004f, clkY, clkX + 0.004f, clkY + clkR * 0.50f, 0.15f, 0.15f, 0.18f);
        drawQuad(sp, VAO, VBO, clkX - 0.003f, clkY - clkR * 0.35f, clkX + 0.003f, clkY, 0.25f, 0.15f, 0.05f);

        //baam side er pichoner dewale notice board
        drawQuad(sp, VAO, VBO, -0.95f, 0.50f, -0.65f, 0.84f, 0.28f, 0.18f, 0.06f);
        drawQuad(sp, VAO, VBO, -0.93f, 0.52f, -0.67f, 0.82f, 0.85f, 0.72f, 0.50f);
        // Papers pinned on board (small colored rectangles)
        drawQuad(sp, VAO, VBO, -0.92f, 0.66f, -0.78f, 0.80f, 0.95f, 0.95f, 0.90f);
        drawQuad(sp, VAO, VBO, -0.76f, 0.66f, -0.68f, 0.80f, 0.85f, 0.92f, 0.96f);
        drawQuad(sp, VAO, VBO, -0.92f, 0.54f, -0.80f, 0.64f, 0.96f, 0.88f, 0.72f);
        drawQuad(sp, VAO, VBO, -0.78f, 0.54f, -0.68f, 0.64f, 0.90f, 0.78f, 0.78f);
        // Push pins (tiny colored dots as small squares)
        drawQuad(sp, VAO, VBO, -0.854f, 0.794f, -0.838f, 0.808f, 0.90f, 0.20f, 0.20f);
        drawQuad(sp, VAO, VBO, -0.714f, 0.794f, -0.698f, 0.808f, 0.20f, 0.55f, 0.85f);
        drawQuad(sp, VAO, VBO, -0.854f, 0.634f, -0.838f, 0.648f, 0.20f, 0.75f, 0.35f);
        drawQuad(sp, VAO, VBO, -0.744f, 0.634f, -0.728f, 0.648f, 0.90f, 0.70f, 0.10f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(sp);
    glfwTerminate();
    return 0;
}

//ei part e shape draw ar shadder e colour oathanor part
void drawShape(unsigned int sp, unsigned int VAO, unsigned int VBO,
    float* verts, int count, float r, float g, float b, float alpha)
{
    int loc = glGetUniformLocation(sp, "uColor");
    glUniform4f(loc, r, g, b, alpha);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * 3 * sizeof(float), verts);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glBindVertexArray(0);
}

//ei part e amra draw shape call korsi
void drawQuad(unsigned int sp, unsigned int VAO, unsigned int VBO,
    float x1, float y1, float x2, float y2,
    float r, float g, float b, float alpha)
{
    float v[] = {
        x1,y1,0, x2,y1,0, x2,y2,0,
        x1,y1,0, x2,y2,0, x1,y2,0,
    };
    drawShape(sp, VAO, VBO, v, 6, r, g, b, alpha);
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}