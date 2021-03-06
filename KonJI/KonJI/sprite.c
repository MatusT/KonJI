#define _CRT_SECURE_NO_WARNINGS

#include "sprite.h"

//format
//[frames max 255][width max 255][height max 255][2bytes(~CHAR_INFO~) <- f*w*h]
struct Sprite* spriteLoad(const unsigned char* file_name) {
	struct Sprite* sprite = malloc(sizeof(struct Sprite));
	FILE *in_file;
	in_file = fopen(file_name, "rb");
	if (!in_file) {
		printf("Unable to open file!\n");
		return NULL;
	}

	unsigned char frames;
	unsigned char width;
	unsigned char height;

	fseek(in_file, 0, SEEK_SET);
	fread(&frames, sizeof(unsigned char), 1, in_file);
	fseek(in_file, 1, SEEK_SET);
	fread(&width, sizeof(unsigned char), 1, in_file);
	fseek(in_file, 2, SEEK_SET);
	fread(&height, sizeof(unsigned char), 1, in_file);

	sprite->frames = frames;
	sprite->w = width;
	sprite->h = height;
	unsigned char* buffer = malloc(sizeof(unsigned char)*sprite->frames*sprite->w*sprite->h * 2);

	fseek(in_file, 3, SEEK_SET);
	fread(buffer, sizeof(unsigned char), sprite->frames*sprite->w*sprite->h * 2, in_file);
	fclose(in_file);

	sprite->bitmap = malloc(sprite->frames*sizeof(CHAR_INFO*));
	for (int i = 0; i < sprite->frames; i++) {
		sprite->bitmap[i] = malloc(sprite->w*sprite->h*sizeof(CHAR_INFO));
	}

	sprite->current_frame = 0;

	int sprite_size = sprite->w * sprite->h;
	for (int i = 0; i < sprite->frames; i++) {
		for (int j = 0; j < sprite_size; j++) {
			sprite->bitmap[i][j].Char.AsciiChar = buffer[i*sprite_size + j*2];
			sprite->bitmap[i][j].Attributes = buffer[i*sprite_size + j*2+1];
		}
	}
	free(buffer);

	return sprite;
}

struct Sprite* spriteCreate(CHAR_INFO** bitmap, char w, char h, char frames) {
	struct Sprite* sprite = malloc(sizeof(struct Sprite));
	
	sprite->bitmap = bitmap;
	sprite->w = w;
	sprite->h = h;
	sprite->frames = frames;
	sprite->current_frame = 0;

	return sprite;
}

void spriteDraw(struct Window* window, struct Sprite* sprite, int x_pos, int y_pos) {
	if (sprite->bitmap == NULL) {
		return;
	}

	for (int y = 0; y < sprite->h; y++) {
		for (int x = 0; x < sprite->w; x++) {
			if (x_pos + x >= window->w || y_pos + y >= window->h ||
				x_pos + x < 0 || y_pos + y < 0) {
				continue;
			}
			if (sprite->bitmap[sprite->current_frame][sprite->w * y + x].Char.AsciiChar != 0 &&
				sprite->bitmap[sprite->current_frame][sprite->w * y + x].Attributes != 0) {
				window->consoleBuffer[x_pos + x + (y_pos + y)*window->w] = sprite->bitmap[sprite->current_frame][sprite->w * y + x];
			}

		}
	}
}

void spriteWrite(struct Sprite* sprite, const unsigned char* file_name) {
	FILE *out_file;

	char* buffer = malloc(sizeof(char)*(2 * sprite->frames*sprite->w*sprite->h + 3));
	buffer[0] = sprite->frames;
	buffer[1] = sprite->w;
	buffer[2] = sprite->h;

	int sprite_size = sprite->w * sprite->h;
	for (int i = 0; i < sprite->frames; i++) {
		for (int j = 0; j < sprite_size; j++) {
			buffer[i*sprite_size + 2*j+3] = sprite->bitmap[i][j].Char.AsciiChar;
			buffer[i*sprite_size + 2*j+4] = sprite->bitmap[i][j].Attributes;
		}
	}

	out_file = fopen(file_name, "wb");
	fwrite(buffer, sizeof(char), 2 * sprite->frames * sprite->w * sprite->h + 2, out_file);

	fclose(out_file);
	free(buffer);
}

CHAR_INFO spriteAt(struct Sprite* sprite, unsigned int x, unsigned int y) {
	return sprite->bitmap[sprite->current_frame][sprite->w * y + x];
}

void spriteFree(struct Sprite* sprite) {
	free(sprite->bitmap);
	free(sprite);
}

struct Vector2d spriteCenterOfMass(struct Sprite* sprite) {
	struct Vector2d ret_vector = { 0.0, 0.0 };
	int n = 0;
	for (unsigned int x = 0; x < sprite->w; x++) {
		for (unsigned int y = 0; y < sprite->h; y++) {

			CHAR_INFO cell = spriteAt(sprite, x, y);
			if (cell.Attributes != 0 && cell.Char.AsciiChar != 0) {
				n++;
				ret_vector.x += x;
				ret_vector.y += y;
			}

		}
	}
	return vector2dScalarMult(1 / n, ret_vector);
}