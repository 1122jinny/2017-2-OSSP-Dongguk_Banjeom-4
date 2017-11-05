/*
 *      shapes.c
 *      Copyright © 2008 Martin Duquesnoy <xorg62@gmail.com>
 *      All rights reserved.
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following disclaimer
 *        in the documentation and/or other materials provided with the
 *        distribution.
 *      * Neither the name of the  nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "tetris.h"

/*  Shapes data */
/*  각각의 4X4X2 배열은 블록 모양의 4가지 경우를 표현한다. 따라서 현재는 7가지 종류의 블록을 가진다.
 *  예를들어, I 모양의 블록을 상하좌우로 돌리는 경우 각각의 상태가 배열에 저장되어 있는것.
 *  X와 Y축이 0~3인 좌표계를 생각하고, {0~3,0~3}의 좌표로 도형을 나타낸다.
 */


const int shapes[7][4][4][2] =
{
     /* O */
     /* 정사각형 */
     {
          {{0,0},{1,0},{0,1},{1,1}},
          {{0,0},{1,0},{0,1},{1,1}},
          {{0,0},{1,0},{0,1},{1,1}},
          {{0,0},{1,0},{0,1},{1,1}}
     },
     /* I */
     /* 일자형 막대 */
     {
          {{1,0},{1,1},{1,2},{1,3}},
          {{0,1},{1,1},{2,1},{3,1}},
          {{1,0},{1,1},{1,2},{1,3}},
          {{0,1},{1,1},{2,1},{3,1}}
     },
     /* L */
     /* 오른쪽 방향 ㄴ자 블록 */
     {
          {{0,1},{1,1},{2,1},{2,2}},
          {{1,0},{1,1},{1,2},{2,0}},
          {{0,0},{0,1},{1,1},{2,1}},
          {{1,0},{1,1},{1,2},{0,2}}
     },
     /* J */
     /* 왼쪽 방향 ㄱ자 블록 */
     {
          {{1,0},{1,1},{1,2},{2,2}},
          {{0,2},{1,2},{2,2},{2,1}},
          {{0,0},{1,0},{1,1},{1,2}},
          {{0,1},{0,2},{1,1},{2,1}}
     },
     /* S */
     /* S자 블록 */
     {
          {{1,1},{1,2},{2,0},{2,1}},
          {{0,1},{1,1},{1,2},{2,2}},
          {{1,1},{1,2},{2,0},{2,1}},
          {{0,1},{1,1},{1,2},{2,2}}
     },
     /* Z */
     /* Z자 블록 */
     {
          {{0,0},{0,1},{1,1},{1,2}},
          {{0,2},{1,1},{2,1},{1,2}},
          {{0,0},{0,1},{1,1},{1,2}},
          {{0,2},{1,1},{2,1},{1,2}}
     },
     /* T */
     /* T 블록 */
     {
          {{0,1},{1,0},{1,1},{1,2}},
          {{0,1},{1,1},{1,2},{2,1}},
          {{1,0},{1,1},{1,2},{2,1}},
          {{1,0},{0,1},{1,1},{2,1}}
     }
};

void
shape_set(void)
{
     int i, j;
     
     /* frame[FRAMEH+1][FRAMEW+1]
        FRAMEH: 프레임의 높이, FRAMEW: 프레임의 넓이
        아래의 반복문에서
        FRAMEH = current.x + shpaes[current.num][current.pos][i][o]
        프레임의 높이는 current.x 값에 현재 출력되고 있는 블록의 값을 더하는 것.
	current.num은 0~6까지 무작위 정수. 
        (ex) currnet.num = 1, current.pos = 2 이면 1자형 막대이다.
	
	본 함수에서는 frame[x][y] 배열에 값을 채워넣는다.
	EXP_FACT = 2이고, 이 값은 frame의 열에 곱해진다.
	정해진 frame[x][y] 위치에 current.num + 1의 값을 저장한다.
	current.y = (FRAMEW / 2) - 1, #define FRAMEW (int)(10*2.3)
     */
     for(i = 0; i < 4; ++i)
          for(j = 0; j < EXP_FACT; ++j)
               frame[current.x + shapes[current.num][current.pos][i][0]]
                    [current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j]
                    = current.num + 1;

     /* 
	current.x의 값이 1보다 작으면
	배열 frame의 모든 첫번째 행에 Border라는 값을 저장한다.
	Border는 색의 값을 저장하는 변수이다.
     */

     if(current.x < 1)
          for(i = 0; i < FRAMEW + 1; ++i)
               frame[0][i] = Border;

     return;
}

void
shape_unset(void)
{
     int i, j;

     for(i = 0; i < 4; ++i)
          for(j = 0; j < EXP_FACT; ++j)
               frame[current.x + shapes[current.num][current.pos][i][0]]
                    [current.y + shapes[current.num][current.pos][i][1] * EXP_FACT + j] = 0;

     if(current.x < 1)
          for(i = 0; i < FRAMEW + 1; ++i)
               frame[0][i] = Border;
     return;
}

void
shape_new(void)
{
     int i;

     /* Draw the previous shape for it stay there */
     shape_set();
     check_plain_line();

     /* Set the new shape property */
     /*
        next 값을 num으로 넘겨 다음 Shape를 설정한다.
        새로운 Shape를 만드는 함수이기 때문에 current.x값을 1로 설정해준다.
        다음 next 값은 0~6 사이의 무작위 정수로 설정된다.
     */
     current.num = current.next;
     current.x = 1;
     current.y = (FRAMEW / 2) - 1;;
     current.next = nrand(0, 6);
     /* 다음 블록을 표시해준느 프레임을 초기화시킨다 */
     frame_nextbox_refresh();
     /* 
	current.x 값이 1보다 크면 
	배열 frame의 두번째 행의 모든 값을 0으로 지정한다
     */        
     if(current.x > 1)
          for(i = 2; i < FRAMEW - 1; ++i)
               frame[1][i] = 0;

     return;
}

void
shape_go_down(void)
{

     shape_unset();

     /* Fall the shape else; collision with the ground or another shape
      * then stop it and create another */
     if(!check_possible_pos(current.x + 1, current.y))
          ++current.x;
     else
          if(current.x > 2)
               shape_new();
          else
          {
               shape_new();
               frame_refresh();
               sleep(2);
               running = False;
          }




     return;
}

void
shape_set_position(int p)
{
     int old = current.pos;

     shape_unset();

     current.pos = p ;

     if(check_possible_pos(current.x, current.y))
          current.pos = old;

     return;
}


void
shape_move(int n)
{

     shape_unset();

     if(!check_possible_pos(current.x, current.y + n))
          current.y += n;

     return;
}

void
shape_drop(void)
{
     while(!check_possible_pos(current.x + 1, current.y))
     {
          shape_unset();
          ++current.x;
     }
     score += FRAMEH - current.x;

     return;
}
