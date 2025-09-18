#include <iostream>
#include <SDL3/SDL.h>
#include <cmath>

const int WIN_HEIGHT = 600;
const int WIN_WIDTH = 800;
const int CircleX = WIN_WIDTH / 2;
const int CircleY = WIN_HEIGHT / 2;
const int Radius = 100;
const int RS = 100;
const float GM = 2500;
const int u_dc = 1;

struct Point{

	void Create(float newX, float newY, float newVX, float newVY);
	void CalculateDilation();
	float CalculateDistance();
	void ApplyGravity();
	void KillPoint();

	bool exists = true;
	float x, y;
	float r, g, b, a, dc;
	float d;
	float vx, vy;
};

void Point::Create(float newX, float newY, float newVX, float newVY){
	x = newX;
	y = newY;
	r = 255;
	g = 255;
	b = 255;
	a = 255;
	dc = u_dc;
	d = CalculateDistance();
	vx = newVX;
	vy = newVY;
}

void Point::CalculateDilation(){
	dc = std::sqrt(1 - (RS / d)) * u_dc;
}

float Point::CalculateDistance(){
	float dx = CircleX - x;
	float dy = CircleY - y;

	float dist = std::sqrt((dx * dx) + (dy * dy));
	
	d = dist;

	return dist;
}

void Point::ApplyGravity() {
	float dx = CircleX - x;
	float dy = CircleY - y;

	if(d > 0.0f) {
    		float a = GM / (d * d);
    		vx += a * (dx / d);  
    		vy += a * (dy / d); 
	}
	x += vx;
	y += vy;
}

void Point::KillPoint(){
	exists = false;
	r = 0;
	g = 0;
	b = 0;
	dc = 0;
	d = 0;
	vx = 0;
	vy = 0;
}

void DrawFullCircle(SDL_Renderer *renderer, int x, int y, int radius)
{
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      SDL_RenderPoint(renderer, centreX + x, centreY - y);
      SDL_RenderPoint(renderer, centreX + x, centreY + y);
      SDL_RenderPoint(renderer, centreX - x, centreY - y);
      SDL_RenderPoint(renderer, centreX - x, centreY + y);
      SDL_RenderPoint(renderer, centreX + y, centreY - x);
      SDL_RenderPoint(renderer, centreX + y, centreY + x);
      SDL_RenderPoint(renderer, centreX - y, centreY - x);
      SDL_RenderPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

int main(int argc, char* argv[]){

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("window", WIN_WIDTH, WIN_HEIGHT, 0);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	SDL_Event evt;
	bool run = true;

	float r = 255, g = 255, b = 255, a = 255, dc = u_dc;

	int size = 100;

	Point points[size];

	/*
	points[0].Create(CircleX + 150, CircleY, 0, 4.08);
	points[1].Create(CircleX + 300, CircleY, 0, std::sqrt(GM/300)); */

	for(int i = 0; i < size; i++){
		points[i].Create(0, i * ((WIN_HEIGHT - 100) / size) + 50, std::sqrt(GM/300), 0);
	} 

	while(run){
		while(SDL_PollEvent(&evt)){
			if(evt.type == SDL_EVENT_QUIT){
				run = false;
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderLine(renderer, WIN_WIDTH - 10, 0, WIN_WIDTH - 10, WIN_HEIGHT);
		if(r > 0){
			r -= dc;
			g -= dc;
			b -= dc;
		}
		
		for(int i = 0; i < size; i++){
			if(!points[i].exists) continue;
			SDL_SetRenderDrawColor(renderer, points[i].r, points[i].g, points[i].b, 255);
			//DrawFullCircle(renderer, points[i].x, points[i].y, 5);
			SDL_RenderPoint(renderer, points[i].x, points[i].y);

			if(points[i].r > 0){
				points[i].r -= points[i].dc;
				points[i].g -= points[i].dc;
				points[i].b -= points[i].dc;
			}
			else points[i].KillPoint();

			if(points[i].d <= Radius) points[i].KillPoint();

			points[i].CalculateDistance();
			points[i].CalculateDilation();
			points[i].ApplyGravity();
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		DrawCircle(renderer, CircleX, CircleY, Radius);

		SDL_RenderPresent(renderer);
		SDL_Delay(16);
	}

	return 0;
}

