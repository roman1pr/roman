#include "box.h"
#include "shader.h"


Box::Box(QWidget *parent) :
    QGLWidget(parent),
    mWindowFlag(false),
    mCameraSpeed(0.3f),
    mModel(1.0f),  //component MPV uses in scroll
    mEye(0.0f,1.0f,5.0f),  // setting for camera
    mCenter(1.0f,0.0f,-1.0f),
    mCameraUp(0.0f,1.0f,0.0f)
{
    //for mouse
    setFocus();
    setMouseTracking(true);

    //Timer
    connect(&mTimer, SIGNAL(timeout()), SLOT(updateGL()));
    connect(&mTimer2, SIGNAL(timeout()), SLOT(checkMouseCoord()));
    mTimer.start(24);
    mTimer2.start(25);



    mVertexData.push_back( glm::vec3(-1.0f, 1.0f, 1.0f) ); //1
    mVertexData.push_back( glm::vec3( 1.0f,1.0f,1.0f) );
    mVertexData.push_back( glm::vec3(1.0f,-1.0f,1.0f) );
    mVertexData.push_back( glm::vec3(-1.0f,-1.0f,1.0f) );

    mVertexData.push_back( glm::vec3(1.0f, 1.0f, -1.0f) ); //2
    mVertexData.push_back( glm::vec3(1.0f,-1.0f,-1.0f) );
    mVertexData.push_back( glm::vec3(-1.0f,1.0f, -1.0f) );
    mVertexData.push_back( glm::vec3(-1.0f,-1.0f,-1.0f) );

    mIndices =
    {
        0,1,2,
        2,3,0,

        1,4,5,
        5,2,1,

        4,6,7,
        7,5,4,

        6,0,3,
        3,7,6,

        6,4,1,
        1,0,6,

        3,2,5,
        5,7,3
    };

    mVertexDataGround.push_back( glm::vec3(-50.0f, -1.0f,-50.0f) ); //down
    mVertexDataGround.push_back( glm::vec3(50.0f, -1.0f, -50.0f) );
    mVertexDataGround.push_back( glm::vec3(50.0f,-1.0f, 50.0f) );
    mVertexDataGround.push_back( glm::vec3(-50.0f,-1.0f, 50.0f) );

    for (int i = 0; i < 1; i++){ //BoxTexture
        mTextData.push_back( glm::vec2 (0.0f, 1.0f) );
        mTextData.push_back( glm::vec2 (1.0f, 1.0f) );
        mTextData.push_back( glm::vec2 (1.0f, 0.0f) );
        mTextData.push_back( glm::vec2 (0.0f, 0.0f) );
    }

    for (int i = 0; i < 1; i++){ //GroundTexture
        mTextDataGround.push_back( glm::vec2 (0.0f, 1.0f) );
        mTextDataGround.push_back( glm::vec2 (1.0f, 1.0f) );
        mTextDataGround.push_back( glm::vec2 (1.0f, 0.0f) );
        mTextDataGround.push_back( glm::vec2 (0.0f, 0.0f) );
    }


    mIndicesGround = {
        0, 1, 2,
        2, 3, 0
    };

    for (int i = 0; i < 100; i+=2){
        for (int j = 0; j < 100; j+=2)
        mBoxPositions[i%2+1][j%2] = 1;
    }

    int k;
    for (int i = 0; i < 100; i++){ //random vector for instances

       glm::vec3 translation;
       translation.x = qrand() % 99;
       translation.z = qrand() % 99;
       translation.y = 1;

       if (mBoxPositions[(int)translation.x][(int)translation.z] == 0) {
            translation.x -= 49;
            translation.z -= 49;
            mTranslations[i] =  translation;

       }
       else {
        qDebug() << k++;
       }
    }

    //color Box

    mBoxColor.push_back(glm::vec4(1.0f,1.0f,1.0f,0.6f));
    for (int i = 0; i < 100; i++){
            mBoxColor.push_back(glm::vec4( qrand()%10,qrand()%10,qrand()%10,0.99f));
    }
}

void Box::paintGL()
{

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(mModelLoc, 1, GL_FALSE, glm::value_ptr(mModel));
    glUniformMatrix4fv(mViewLoc, 1, GL_FALSE, glm::value_ptr(mView));
    glUniformMatrix4fv(mProjectionLoc, 1, GL_FALSE, glm::value_ptr(mProjection));

    // paint boxes
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 100);

    //  ready for another object to draw
    glBindVertexArray(0);

    //for ground texture
    glBindTexture(GL_TEXTURE_2D, mTexGround);
    glBindVertexArray(VAOGround);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //camera
    mView = glm::lookAt(
        mEye,
        mEye + mCenter,
        mCameraUp
    );
}

Box::~Box()
{
    glDeleteVertexArrays(1, &VAO);
}

void Box::resizeGL(int w, int h){
    glViewport(0,0,w,h);
}


void Box::initializeGL()
{
    glewInit();

    Shader ourShader("/opt/qtcreator-2.7.2/Box10/VertexShader.sh", "/opt/qtcreator-2.7.2/Box10/FragmentShader.sh");
    ourShader.Use();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glDepthFunc(GL_ALWAYS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Buffers Box

    glGenVertexArrays(1, &VAO);
    glGenBuffers(3, mVertexBuffer);
    glGenBuffers(1, &EBO);
    glGenTextures(1, &mTex);
    glGenBuffers(1, &instanceVBO);
    glBindVertexArray(VAO);

    // for index Box

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndices), mIndices, GL_STATIC_DRAW);

    // for coords Box

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[0]);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, mVertexData.size() * sizeof(glm::vec3), mVertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    // for texture Box

//    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[1]);
//    glEnableVertexAttribArray(2);
//    glBufferData(GL_ARRAY_BUFFER, mTextData.size() * sizeof(glm::vec2), mTextData.data(), GL_STATIC_DRAW);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

//    QImage image1(":/new/sample4.jpg");

//    glBindTexture(GL_TEXTURE_2D, mTex);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image1.width(), image1.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image1.bits());
//    glGenerateMipmap(GL_TEXTURE_2D);

    // for instances

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glEnableVertexAttribArray(3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 100, &mTranslations[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribDivisor(3, 1);

    // for Color Box

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[2]);
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER, mBoxColor.size() * sizeof(glm::vec4), mBoxColor.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glVertexAttribDivisor(1, 1);
    // Buffers Ground

    glGenVertexArrays(1, &VAOGround);
    glGenBuffers(3, mVertexBufferGround);
    glGenBuffers(1, &EBOGround);
    glGenTextures(1, &mTexGround);
    glBindVertexArray(VAOGround);

    // for index Ground

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOGround);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mIndicesGround), mIndicesGround, GL_STATIC_DRAW);

    // for coords Ground

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferGround[0]);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, mVertexDataGround.size() * sizeof(glm::vec3), mVertexDataGround.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);


    // for texture Ground

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferGround[1]);
    glEnableVertexAttribArray(2);
    glBufferData(GL_ARRAY_BUFFER, mTextDataGround.size() * sizeof(glm::vec2), mTextDataGround.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer[2]);
    glEnableVertexAttribArray(1);
    glBufferData(GL_ARRAY_BUFFER, mBoxColor.size() * sizeof(glm::vec4), mBoxColor.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);

    QImage image2(":/new/sample5.jpg");

    glBindTexture(GL_TEXTURE_2D, mTexGround);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image2.width(), image2.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, image2.bits());
    glGenerateMipmap(GL_TEXTURE_2D);

    // shader

    mModelLoc = glGetUniformLocation(ourShader.Program, "model");
    mViewLoc = glGetUniformLocation(ourShader.Program, "view");
    mProjectionLoc = glGetUniformLocation(ourShader.Program, "projection");

    // Projection for camera

    mProjection = glm::perspective(
                45.0f,         // Горизонтальное поле обзора в градусах. Обычно между 90° (очень широкое) и 30° (узкое)
                (float) width() / height(), // Отношение сторон. Зависит от размеров вашего окна. Заметьте, что 4/3 == 800/600 == 1280/960
                0.1f,        // Ближняя плоскость отсечения. Должна быть больше 0.
                150.0f       // Дальняя плоскость отсечения.
            );
}

void Box::keyPressEvent(QKeyEvent* e){

    switch(e->key())
            {
            case Qt::Key_Up:
                mEye += mCameraSpeed * mCenter;
                break;
            case Qt::Key_Down:
                mEye -= mCameraSpeed * mCenter;
                break;
            case Qt::Key_Left:
                mEye -= glm::normalize(glm::cross(mCenter, mCameraUp)) * mCameraSpeed;
                break;
            case Qt::Key_Right:
                mEye += glm::normalize(glm::cross(mCenter, mCameraUp)) * mCameraSpeed;
            break;        mWindowFlag = true;
            }

    mView = glm::lookAt(
        mEye,
        mEye + mCenter,
        mCameraUp
    );
}

void Box::mouseMoveEvent(QMouseEvent *e){

    QPoint MouseCoord = e->pos();

    if (!mWindowFlag){
        mLastX = e->x();
        mLastY = e->y();
    }

    if (mWindowFlag){
        GLfloat xoffset = MouseCoord.x() - mLastX;
        GLfloat yoffset = mLastY - MouseCoord.y(); // Reversed since y-coordinates range from bottom to top
        mLastX = MouseCoord.x();
        mLastY = MouseCoord.y();

        GLfloat sensitivity = 0.18f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        mYaw   += xoffset;
        mPitch += yoffset;

        if(mPitch > 89.0f)
          mPitch =  89.0f;
        if(mPitch < -89.0f)
          mPitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
        front.y = sin(glm::radians(mPitch));
        front.z = cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));
        mCenter = glm::normalize(front);
    }


    if (e->x() > width()-20 || e->x() < 20 || e->y() > height()-20 || e->y() < 20){
        mWindowFlag = false;
    }
    else mWindowFlag = true;

}

void Box::checkMouseCoord(){

    if ((mLastX > width() - 20  &&  mLastX < width()) || (mLastX > 0  && mLastX < 20) || (mLastY > height()-20 && mLastY < height()) || (mLastY > 0 &&  mLastY < 20 ))
    {
        if (mLastX > width() - 20  &&  mLastX < width()){
            mYaw   += 0.8;
        }
        if (mLastX > 0  && mLastX < 20)  {
             mYaw  -= 0.8;
        }
        if (mLastY > height()-20 && mLastY < height()) {
            mPitch -= 0.8;
        }
        if (mLastY > 0 &&  mLastY < 20 ) {
            mPitch += 0.8;
        }

        GLfloat sensitivity = 0.02f;

        if(mPitch > 89.0f)
          mPitch =  89.0f;
        if(mPitch < -89.0f)
          mPitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(mPitch)) * cos(glm::radians(mYaw));
        front.y = sin(glm::radians(mPitch));
        front.z = cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));
        mCenter = glm::normalize(front);
    }
}

void Box::wheelEvent(QWheelEvent *event)
{
    if ( event->delta() > 0 ) {
        mModel = glm::scale(mModel, glm::vec3(1.1f));
    }
    else {
        mModel = glm::scale(mModel, glm::vec3(0.9f));
    }
}


