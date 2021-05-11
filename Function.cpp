
#include "Header.h"

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* gTexture = NULL;



bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTexture = loadTexture( "GameOver.png" );
	if( gTexture == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

void Gameover()
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
          bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
				}



				//Clear screen
				SDL_RenderClear( gRenderer );

				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

}



void FillBackground(SDL_Renderer* renderer)
{

	SDL_SetRenderDrawColor(renderer, 255, 50, 80, 0);
	SDL_RenderClear(renderer);
}
void DropFood(SDL_Renderer* renderer, SDL_Rect rectangle)
{
	SDL_SetRenderDrawColor(renderer, 30, 25, 255, 100);
	SDL_RenderFillRect(renderer, &rectangle);
	SDL_RenderPresent(renderer);
}

void GameMode1()
{
	//Khởi tạo thư viện(Khởi động)
	SDL_Init(SDL_INIT_EVERYTHING);
	srand((unsigned int)time(NULL));
	SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Event e;
	Snake snake;
	SDL_Rect food;
	food.w = 20;
	food.h = 20;
	food.x = rand() % (SCREEN_WIDTH / 20)* 20;
	food.y = rand() % (SCREEN_HEIGHT / 20)* 20;
	bool running = true;
	while (running)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) {
				running = false;
			}
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					running = false;
					break;
				case SDLK_w: case SDLK_UP:
					snake.setDirection(0, -1);
					FillBackground(renderer);//tô nền
					SDL_Delay(100);
					snake.Move();
					snake.ShowSnake(renderer);
                    if (snake.EatBody(snake))
                    {
                       Gameover();
                       while (SDL_PollEvent(&e))
		                 {
			               if (e.type == SDL_QUIT)
				               {

				                  close();
				               }

			               else if (e.type == SDL_KEYDOWN)
			               {
				               switch (e.key.keysym.sym)
				                       {
				                   case SDLK_ESCAPE:

					                       close();
					                        break;
                                       }
			               }
		                 }
		                 break;
                     }
					if (snake.isEat(food))
					{
						snake.GrowUp();
						food.x = rand() % (SCREEN_WIDTH / 20)* 20;
						food.y = rand() % (SCREEN_HEIGHT / 20)* 20;
						DropFood(renderer, food);
					}
					DropFood(renderer, food);
					break;
				case SDLK_s: case SDLK_DOWN:
					snake.setDirection(0, 1);
					FillBackground(renderer);//tô nền
					SDL_Delay(100);
					snake.Move();
					snake.ShowSnake(renderer);
					if (snake.EatBody(snake))
                    {
                       Gameover();
                       while (SDL_PollEvent(&e))
		                 {
			               if (e.type == SDL_QUIT)
				               {
				                  close();
				               }

			               else if (e.type == SDL_KEYDOWN)
			               {
				               switch (e.key.keysym.sym)
				                       {
				                   case SDLK_ESCAPE:
					                       close();
					                        break;
                                       }
			               }
		                 }
		                 break;
                    }
					if (snake.isEat(food))
					{
						snake.GrowUp();
						food.x = rand() % (SCREEN_WIDTH / 20)* 20;
						food.y = rand() % (SCREEN_HEIGHT / 20)* 20;
						DropFood(renderer, food);
					}
					DropFood(renderer, food);
					break;
				case SDLK_a: case SDLK_LEFT:
					snake.setDirection(-1, 0);
					FillBackground(renderer);//tô nền
					SDL_Delay(100);
					snake.Move();
					snake.ShowSnake(renderer);
					if (snake.EatBody(snake))
                    {
                       Gameover();
                       while (SDL_PollEvent(&e))
		                 {
			               if (e.type == SDL_QUIT)
				               {

				                  close();
				               }

			               else if (e.type == SDL_KEYDOWN)
			               {
				               switch (e.key.keysym.sym)
				                       {
				                   case SDLK_ESCAPE:

					                       close();
					                        break;
                                       }
			               }
		                 }
		                 break;
                    }
					if (snake.isEat(food))
					{
						snake.GrowUp();
						food.x = rand() % (SCREEN_WIDTH / 20)* 20;
						food.y = rand() % (SCREEN_HEIGHT / 20)* 20;
						DropFood(renderer, food);
					}
					DropFood(renderer, food);
					break;
				case SDLK_d: case SDLK_RIGHT:
					snake.setDirection(1, 0);
					FillBackground(renderer);//tô nền
					SDL_Delay(100);
					snake.Move();
					snake.ShowSnake(renderer);
					if (snake.EatBody(snake))
                    {
                       Gameover();
                       while (SDL_PollEvent(&e))
		                 {
			               if (e.type == SDL_QUIT)
				               {

				                  close();
				               }

			               else if (e.type == SDL_KEYDOWN)
			               {
				               switch (e.key.keysym.sym)
				                       {
				                   case SDLK_ESCAPE:

					                       close();
					                        break;
                                       }
			               }
		                 }
		                 break;
                    }
					if (snake.isEat(food))
					{
						snake.GrowUp();
						food.x = rand() % (SCREEN_WIDTH / 20)* 20;
						food.y = rand() % (SCREEN_HEIGHT / 20)* 20;
						DropFood(renderer, food);
					}
					DropFood(renderer, food);
					break;
				}
			}
		}

		FillBackground(renderer);
		SDL_Delay(100);
		snake.Move();
		snake.ShowSnake(renderer);
		if (snake.EatBody(snake))
        {
            Gameover();
           while (SDL_PollEvent(&e))
		                 {
			               if (e.type == SDL_QUIT)
				               {
				                  running = false;
				                  close();
				               }

			               else if (e.type == SDL_KEYDOWN)
			               {
				               switch (e.key.keysym.sym)
				                       {
				                   case SDLK_ESCAPE:
					                       running = false;
					                       close ();
					                        break;
                                       }
			               }
		                 }
		                 break;
        }
		if (snake.isEat(food))
		{
			snake.GrowUp();
			food.x = rand() % (SCREEN_WIDTH / 20)* 20;
			food.y = rand() % (SCREEN_HEIGHT / 20)* 20;
			DropFood(renderer, food);
		}
		DropFood(renderer, food);
	}
	close();
}


