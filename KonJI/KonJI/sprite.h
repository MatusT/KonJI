#pragma once

#include <windows.h>
#include "window.h"
#include "vector2d.h"
#include <stdio.h>

struct Sprite {
	char frames;
	char w;
	char h;

	char current_frame;
	CHAR_INFO** bitmap;
};

//format
//[frames max 255][width max 255][height max 255][2bytes(~CHAR_INFO~) <- f*w*h]
struct Sprite* spriteLoad(const unsigned char* file_name);
struct Sprite* spriteCreate(CHAR_INFO** bitmap, char w, char h, char frames);
void spriteDraw(struct Window* window, struct Sprite* sprite, int x_pos, int y_pos);
void spriteWrite(struct Sprite* sprite, const unsigned char* file_name);
void spriteFree(struct Sprite* sprite);
CHAR_INFO spriteAt(struct Sprite* sprite, unsigned int x, unsigned int y);

struct Vector2d spriteCenterOfMass(struct Sprite* sprite);