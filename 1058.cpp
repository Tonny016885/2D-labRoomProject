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