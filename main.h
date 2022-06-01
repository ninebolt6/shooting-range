#pragma once

/**
 * @brief 描画の命令を処理するコールバック関数。
 * 
 * @note 実装の最後に必ず#glFlush()や#glutSwapBuffers()を呼び出す。
 */
void display(void);

/**
 * @brief プログラムのアイドル時に呼び出されるコールバック関数。
 * 
 * @note #glutPostRedisplay()を呼ばないとdisplayコールバック関数が呼び出されない。
 */
void idle(void);

/**
 * @brief ウインドウのリサイズ時に呼び出されるコールバック関数。
 * 
 * @param width ウインドウの幅
 * @param height ウインドウの高さ
 * @note 初回描画時(プログラム起動時)にも1回呼び出される。
 */
void reshape(int width, int height);

/**
 * @brief マウスドラッグに関するイベントを処理するコールバック関数。
 * 
 * @param x マウスのX座標
 * @param y マウスのY座標
 * @note マウスボタンを押下中にマウスを動かした場合に呼ばれる。
 */
void motion(int x, int y);

/**
 * @brief マウスボタンに関するイベントを処理するコールバック関数。
 * 
 * @param button 押されたボタンの番号(GLUT_LEFT_BUTTON | GLUT_MIDDLE_BUTTON | GLUT_RIGHT_BUTTON)
 * @param state 状態(GLUT_DOWN | GLUT_UP)
 * @param x ウインドウ内のマウスのX座標
 * @param y ウインドウ内のマウスのY座標
 * @note マウスホイールの動作でも発火する(ホイール上 = 3, ホイール下 = 4)。
 */
void mouse(int button, int state, int x, int y);

/**
 * @brief キーボードの特殊キーに関するイベントを処理するコールバック関数。
 * 
 * @param key 押された特殊キーの番号
 * @param x イベント発火時のマウスのX座標
 * @param y イベント発火時のマウスのY座標
 * @see https://www.opengl.org/resources/libraries/glut/spec3/node54.html
 */
// void specialKey(int key, int x, int y);

/**
 * @brief キーボードの特殊キーUpに関するイベントを処理するコールバック関数。
 * 
 * @param key 離された特殊キーの番号
 * @param x イベント発火時のマウスのX座標
 * @param y イベント発火時のマウスのY座標
 * @see https://www.opengl.org/resources/libraries/glut/spec3/node54.html
 */
// void specialKeyUp(int key, int x, int y);

/**
 * @brief キーボードのキーに関するイベントを処理するコールバック関数。
 * 
 * @param key 押されたキー
 * @param x イベント発火時のマウスのX座標
 * @param y イベント発火時のマウスのY座標
 * @see https://www.opengl.org/resources/libraries/glut/spec3/node49.html
 * @note ファンクションキー等の一部のキーは#specialKey()コールバック関数で処理できる。
 */
void keyboard(unsigned char key, int x , int y);

/**
 * @brief キーボードのキーUpに関するイベントを処理するコールバック関数。
 * 
 * @param key 離されたキー
 * @param x イベント発火時のマウスのX座標
 * @param y イベント発火時のマウスのY座標
 * @see https://www.opengl.org/resources/libraries/glut/spec3/node49.html
 * @note ファンクションキー等の一部のキーは#specialKeyUp()コールバック関数で処理できる。
 */
// void keyboardUp(unsigned char key, int x , int y);

void pickUp(int x, int y);
void init(char *progname);
void initState();
void initTarget();
void timer(int value);
void drawGround();
void renderString(const std::string &str, float x, float y);