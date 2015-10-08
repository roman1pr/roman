#ifndef BOX_H
#define BOX_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QVector>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <QTimer>
#include <QImage>
#include "shader.h"
#include <QKeyEvent>

class Box : public QGLWidget
{

    Q_OBJECT

public:
    Box(QWidget *parent = 0);
    virtual ~Box();
    void keyPressEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *event);

protected:
    void paintGL();
    void initializeGL();
    void resizeGL(int w, int h);


protected slots:
        void checkMouseCoord();

private:
    //for coords and textures for Box
    GLuint mTex, uiSampler;
    QVector < glm::vec3 > mVertexData;
    QVector < glm::vec2 > mTextData;
    GLuint mIndices[6*3*2];
    GLuint VAO, EBO, mVertexShader, mFragmentShader, mShaderProgram, mVertexBuffer[3], iSamplerLoc;

    // for color

    QVector <glm::vec4> mBoxColor;

    //for instances
    glm::vec3 mTranslations[100];
    GLuint instanceVBO;

    //for coords and textures for ground
    GLuint VAOGround, mVertexBufferGround[3], EBOGround, mTexGround;
    QVector < glm::vec3 > mVertexDataGround;
    QVector < glm::vec2 > mTextDataGround;
    GLuint mIndicesGround[1*3*2];
    int mBoxPositions[100][100];


    //for camera
    GLuint mModelLoc, mProjectionLoc, mViewLoc;

    glm::mat4 mProjection;
    glm::mat4 mModel;
    glm::mat4 mView;

    glm::vec3 mEye;
    glm::vec3 mCenter;
    glm::vec3 mCameraUp;

    GLfloat mCameraSpeed;

    //for moving

    QTimer mTimer;

    QTimer mTimer2;
    bool mNeedMove;
    GLfloat mYaw,mPitch;

    GLfloat mLastX, mLastY;

    //for catching mouse
    bool mWindowFlag;
    bool mFirstInputInWindow;

};

#endif // BOX_H
