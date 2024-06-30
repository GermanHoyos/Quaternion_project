// THE  SOURCE   - WHO WAS, WHO IS, WHO IS COMING: YHWH
//
// GERMAN HOYOS - IMAGINATION AND WILLPOWER ARE THE ONLY TWO CODING SKILLS THAT MATTER. TWITTER||X: //https://x.com/EvangelizerH
//
// GAME START   - [X][Y][Z]: FROM THE PERSPECTIVE OF THE INIT SCREEN START, ZOOM AND ROTATE WITH "P" KEY 
//                                [Y]              [Z] 
//                                **              **
//                                **            **
//                                **          **
//                                **        **
//                                **      **
//                                **    **
//                                **  **
//                                ****
//      ******************************************************[X]
//                              ****
//                            **  **
//                          **    **
//                        **      **
//                      **        **
//                    **          **
//                  **            **
//                **              **
//
// OPENGL 4X4 IDENTITY MATRIX
// | 1  0  0  0 |  ->  | m0 m4 m8 m12 |   // m0, m1, m2: First column, representing the x-axis direction vector.
// | 0  1  0  0 |  ->  | m1 m5 m9 m13 |   // m4, m5, m6: Second column, representing the y-axis direction vector.
// | 0  0  1  0 |  ->  | m2 m6 m10 m14|   // m8, m9, m10: Third column, representing the z-axis direction vector.
// | 0  0  0  1 |  ->  | m3 m7 m11 m15|   // m12, m13, m14: Translation components along the x, y, and z axes, respectively.
//
// OPENGL + RAYLIB "MATRIX" STRUCT
// | 1  0  0  10 |  ->  | m0  m4  m8  m12 |    // [12] = x-axis
// | 0  1  0  0  |  ->  | m1  m5  m9  m13 |    // [13] = y-axis
// | 0  0  1  0  |  ->  | m2  m6  m10 m14 |    // [14] = z-axis
// | 0  0  0  1  |  ->  | m3  m7  m11 m15 |    // [15] = always 1, required for matrix multiplication
//
// AXIS IN RELATION TO RAYLIB DEFAULT CAMERA POSITION
// [13][Y] = (+)up     (-)down
// [12][X] = (+)right  (-)left
// [14][Z] = (-)foward (+)back


// class camera
// {
//    public:
//    Vector3 pos;
//    Quaternion rot;

//    void moveFwrd(float meters)
//    {
//       Vector3 fuv = rot.getFwrdUnitVec();
//       fuv *= meters;

//       pos += fuv;
//    }

//    void strafe(float meters)
//    {
//       Vector3 luv = rot.getLatUnitVec();
//       luv *= meters;

//       pos += luv;
//    }

//    void moveVert(float meters)
//    {
//       Vector3 uuv = rot.getUpUnitVec();
//       uuv *= meters;

//       pos += uuv;
//    }

//    // RMAPI Quaternion QuaternionFromEuler(float pitch, float yaw, float roll)
//    void turnRight(float degrees)
//    {
//       Quaternion q = QuaternionFromEuler(0,degrees,0);
//       rot = q * rot;
//    }


// };