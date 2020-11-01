//Assignment 02: Trandformations, viewing and projection

#include "utils.h"

int width = 640, height=640;
#define DIVISIONS 1000
#define PI 3.14

int main(int, char**)
{
    // Setup window
    GLFWwindow *window = setupWindow(width, height);
    ImGuiIO& io = ImGui::GetIO(); // Create IO object

    ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    //Phong Shading
    //unsigned int shaderProgram = createProgram("./shaders/vshader2.vs", "./shaders/fshader2.fs");
    
    //Gouraud Shading
    unsigned int shaderProgram = createProgram("./shaders/vshader1.vs", "./shaders/fshader.fs");
    
    glUseProgram(shaderProgram);



    setupModelTransformation(shaderProgram);
    setupViewTransformation(shaderProgram);
    setupProjectionTransformation(shaderProgram, width , height);
    
    int vVertex_attrib = glGetAttribLocation(shaderProgram, "vVertex");
    if(vVertex_attrib == -1) {
        std::cout << "Could not bind location: vVertex\n" ;
        exit(0);
    }else{
        std::cout << "vVertex found at location " << vVertex_attrib << std::endl;
    }

    // int vNormal_attrib = glGetAttribLocation(shaderProgram, "vertex_norm");
    // if(vNormal_attrib == -1) {
    //     std::cout << "Could not bind location: vertex_norm\n" ;
    //     exit(0);
    // }else{
    //     std::cout << "aNormal found at location " << vNormal_attrib << std::endl;
    // }



    int nVertices = 0;
    //Looping over to find points
    std::vector<std::vector<GLfloat>> vertexVector;
    GLfloat r = 0.8;

    //We first loop over each latitude and inside it over each longitude and store it inside a 2d matrix, we separate into angles of 5
    for(int thetaIter = 0; thetaIter<=180; thetaIter = thetaIter + 5){
        
        std::vector<GLfloat> currVector;
        
        for(int aziIter = 0; aziIter<=360; aziIter = aziIter + 5){

            GLfloat theta = 1*thetaIter;
            GLfloat azi = 1*aziIter;
            
            //Finding x,y,x coordinates
            GLfloat x = r*sin(theta*PI/180)*cos(azi*PI/180);
            GLfloat y = r*sin(theta*PI/180)*sin(azi*PI/180);
            GLfloat z = r*cos(theta*PI/180);
            currVector.push_back(x);
            currVector.push_back(y);
            currVector.push_back(z);
            
        }

        vertexVector.push_back(currVector);

    }


    //2d vecotr to 1d vector by selecting appropriate points
    std::vector<GLfloat> finalVector;
    int numRows = vertexVector.size();
    int numCols = vertexVector[0].size();

    std::cout<<numRows<<std::endl;
    std::cout<<numCols<<std::endl;
    
    

    for(int row = 0; row<numRows; row = row + 1){
        for(int col = 0; col<numCols; col = col + 3){

            int firstInd = row;
            int secondInd = col;

            int firstIndNext = row+1;
            int secondIndNext = col+3;

            //Checking if we have reached last iteration in which cases we need to connect first and last
            if(firstInd==numRows-1){
                firstIndNext = 0;
            }

            if(secondInd==numCols-3){
                secondIndNext = 0;
            }
            
            GLfloat point1X = vertexVector[firstInd][secondInd];
            GLfloat point1Y = vertexVector[firstInd][secondInd+1];
            GLfloat point1Z = vertexVector[firstInd][secondInd+2];

            GLfloat point2X = vertexVector[firstInd][secondIndNext];
            GLfloat point2Y = vertexVector[firstInd][secondIndNext+1];
            GLfloat point2Z = vertexVector[firstInd][secondIndNext+2];
            
            GLfloat point3X = vertexVector[firstIndNext][secondIndNext];
            GLfloat point3Y = vertexVector[firstIndNext][secondIndNext+1];
            GLfloat point3Z = vertexVector[firstIndNext][secondIndNext+2];
            
            GLfloat point4X = vertexVector[firstInd][secondInd];
            GLfloat point4Y = vertexVector[firstInd][secondInd+1];
            GLfloat point4Z = vertexVector[firstInd][secondInd+2];

            GLfloat point5X = vertexVector[firstIndNext][secondIndNext];
            GLfloat point5Y = vertexVector[firstIndNext][secondIndNext+1];
            GLfloat point5Z = vertexVector[firstIndNext][secondIndNext+2];

            GLfloat point6X = vertexVector[firstIndNext][secondInd];
            GLfloat point6Y = vertexVector[firstIndNext][secondInd+1];
            GLfloat point6Z = vertexVector[firstIndNext][secondInd+2];


            // std::cout<<point1<<std::endl;
            // std::cout<<point2<<std::endl;
            // std::cout<<point3<<std::endl;
            // std::cout<<point4<<std::endl;
            // std::cout<<point5<<std::endl;
            // std::cout<<point6<<std::endl;
            

            finalVector.push_back(point1X);
            finalVector.push_back(point1Y);
            finalVector.push_back(point1Z);

            finalVector.push_back(point2X);
            finalVector.push_back(point2Y);
            finalVector.push_back(point2Z);

            finalVector.push_back(point3X);
            finalVector.push_back(point3Y);
            finalVector.push_back(point3Z);
            
            finalVector.push_back(point4X);
            finalVector.push_back(point4Y);
            finalVector.push_back(point4Z);
            
            finalVector.push_back(point5X);
            finalVector.push_back(point5Y);
            finalVector.push_back(point5Z);

            finalVector.push_back(point6X);
            finalVector.push_back(point6Y);
            finalVector.push_back(point6Z);
            
        }
    }

    //Creating an array from the 1d buffer to be sent to the glBufferData function
    nVertices = finalVector.size();
    GLfloat expanded_vertices[nVertices];

    for(int i = 0; i<nVertices; i++){
        expanded_vertices[i] = finalVector[i];
        //std::cout<<expanded_vertices[i]<<std::endl;
    }

    glUseProgram(shaderProgram);

    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(expanded_vertices), expanded_vertices, GL_STATIC_DRAW); //else tri_points
      
    
    GLuint sphere_VAO;
    glGenVertexArrays(1, &sphere_VAO);
    glBindVertexArray(sphere_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glVertexAttribPointer(static_cast<uint>(vVertex_attrib), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(static_cast<uint>(vVertex_attrib));

   


   float angle = 0.0f;
   GLfloat matrix[] = {
    1.0f, 0.0f, 0.0f, 0.0f, // first column
    0.0f, 1.0f, 0.0f, 0.0f, // second column
    0.0f, 0.0f, 1.0f, 0.0f,// third column
     0.0f, 0.0f, 0.0f, 1.0f
    };



    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glUseProgram(shaderProgram);
        showOptionsDialog(shaderProgram, angle, matrix);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Information");                          
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glBindVertexArray(sphere_VAO); 

        glDrawArrays(GL_TRIANGLES, 0, nVertices);//else tri_points   

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
    cleanup(window);

    return 0;
}

void createCubeObject(unsigned int &program, unsigned int &cube_VAO)
{
    glUseProgram(program);

    //Bind shader variables
    int vVertex_attrib = glGetAttribLocation(program, "vVertex");
    if(vVertex_attrib == -1) {
        fprintf(stderr, "Could not bind location: vVertex\n");
        exit(0);
    }
    int vColor_attrib = glGetAttribLocation(program, "vColor");
    if(vColor_attrib == -1) {
        fprintf(stderr, "Could not bind location: vColor\n");
        exit(0);
    }

    //Cube data
    GLfloat cube_vertices[] = {10, 10, 10, -10, 10, 10, -10, -10, 10, 10, -10, 10, //Front
                   10, 10, -10, -10, 10, -10, -10, -10, -10, 10, -10, -10}; //Back
    GLushort cube_indices[] = {0, 2, 3, 0, 1, 2, //Front
                4, 7, 6, 4, 6, 5, //Back
                5, 2, 1, 5, 6, 2, //Left
                4, 3, 7, 4, 0, 3, //Right
                1, 0, 4, 1, 4, 5, //Top
                2, 7, 3, 2, 6, 7}; //Bottom
    GLfloat cube_colors[] = {1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1}; //Unique face colors

    //Generate VAO object
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);

    //Create VBOs for the VAO
    //Position information (data + format)
    int nVertices = 6*2*3; //(6 faces) * (2 triangles each) * (3 vertices each)
    GLfloat *expanded_vertices = new GLfloat[nVertices*3];
    for(int i=0; i<nVertices; i++) {
        expanded_vertices[i*3] = cube_vertices[cube_indices[i]*3];
        expanded_vertices[i*3 + 1] = cube_vertices[cube_indices[i]*3+1];
        expanded_vertices[i*3 + 2] = cube_vertices[cube_indices[i]*3+2];
    }
    GLuint vertex_VBO;
    glGenBuffers(1, &vertex_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vVertex_attrib);
    glVertexAttribPointer(vVertex_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    delete []expanded_vertices;

    //Color - one for each face
    GLfloat *expanded_colors = new GLfloat[nVertices*3];
    for(int i=0; i<nVertices; i++) {
        int color_index = i / 6;
        expanded_colors[i*3] = cube_colors[color_index*3];
        expanded_colors[i*3+1] = cube_colors[color_index*3+1];
        expanded_colors[i*3+2] = cube_colors[color_index*3+2];
    }
    GLuint color_VBO;
    glGenBuffers(1, &color_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
    glBufferData(GL_ARRAY_BUFFER, nVertices*3*sizeof(GLfloat), expanded_colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vColor_attrib);
    glVertexAttribPointer(vColor_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    delete []expanded_colors;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); //Unbind the VAO to disable changes outside this function.
}
