//============================================================================
// Name        : LearnC.cpp
// Author      : Mark
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include<sstream>

// Function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT , NONE };
side branchPositions[NUM_BRANCHES];

int main() {
	sf::VideoMode vm(1920,1080);
	sf::RenderWindow window(vm, "Timber!!", sf::Style::Fullscreen);
	//create a texture to hold a graphic on the GPU
	sf::Texture textureBackground;

	//load a graphic into the texture
        textureBackground.loadFromFile("graphics/background.png");

	//Creating a sprite
	sf::Sprite spriteBackground;

	//attach the texture to hte sprite
	spriteBackground.setTexture(textureBackground);

	//set the postion of background
	spriteBackground.setPosition(0,0);

	//make a tree sprite
	sf::Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	sf::Sprite spriteTree;
	//Attach texture to the sprite
	spriteTree.setTexture(textureTree);
	//Set coordinates of tree
	spriteTree.setPosition(500,-150);

	//make the bee sprite
	sf::Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	sf::Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(1100,500);
	// Set the bee to being active
	bool beeActive=false;
	//set bee flying speed
	float beeSpeed=0.0f;

	//creating 3 cloud which use the same texture
	sf::Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");

	const int NUM_OF_CLOUDS = 6;
	sf::Sprite clouds[NUM_OF_CLOUDS];
	int cloudSpeed[NUM_OF_CLOUDS];
	bool cloudActive[NUM_OF_CLOUDS];

	for (int i = 0; i < NUM_OF_CLOUDS;i++)
	  {
	    clouds[i].setTexture(textureCloud);
	    clouds[i].setPosition(-300 , i * 150);
	    cloudSpeed[i] = 0;
	    cloudActive[i] = false;
	  }
	
	
	
	sf::Clock clock;




	sf::RectangleShape timeBar;
	float timeBarStartWidth = 500;
	float timeBarHeight = 80;
	timeBar.setSize(sf::Vector2f(timeBarStartWidth/2, timeBarHeight));
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition(1200 - timeBarStartWidth/2,780-timeBarHeight/2);

	sf::Time gameTimeTotal;
	float timerRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timerRemaining;


	// The player 
	sf::Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	sf::Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(0,0);

	side playerSide = side::LEFT;

	sf::Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	sf::Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(3000,1000);

	//Axe

	sf::Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	sf::Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
        spriteAxe.setPosition(375 , 630);
	// align axe with tree
	const float AXE_POSITION_LEFT = 375;
	const float AXE_POSITION_RIGHT = 740;

	//flying log
	sf::Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	sf::Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(500,600);

	// variables related to log
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;
       
	
	
	

	


	
	bool pause = true;

	// drawing text
	int score =0;
	sf::Text messageText;
	sf::Text scoreText;

	//chosing a font the to the message
	sf::Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// set the font to message
	messageText.setFont(font);
	scoreText.setFont(font);

	// assign  message
	messageText.setString("Press Enter to start");
	scoreText.setString("Score = 0");

	//set text size
	messageText.setCharacterSize(50);
	scoreText.setCharacterSize(75);

	// set color
	messageText.setFillColor(sf::Color::White);
	scoreText.setFillColor(sf::Color::White);

	//set postion of text
	sf::FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left+
			      textRect.width / 2.0f,
			      textRect.top + textRect.height /2.0f);
	messageText.setPosition(1100 / 2.0f, 600 /2.0f);
	scoreText.setPosition(20,20);


	// Making branches
	sf::Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	// setting texture for each branch
	for (int i = 0; i< NUM_BRANCHES;i++)
	  {
	    branches[i].setTexture(textureBranch);
	    branches[i].setPosition(-2500,-2500);
	      //setting origin to center
	      branches[i].setOrigin(220,20); }
	// Control the player input
	bool acceptInput = false;

	sf::SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	sf::Sound chop;
	chop.setBuffer(chopBuffer);

	sf::SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	sf::Sound death;
	death.setBuffer(deathBuffer);

	sf::SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	sf::Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);


	// only drawing every 100 frame or so, since drawing takes a lot
	// resources
	int SCORE_LAST_DRAWN=0; 

	
	
	
	while (window.isOpen())
	{
	  sf::Event event;
	  while(window.pollEvent(event)){
	      if (event.type == sf::Event::KeyReleased && !pause)
		{
		  acceptInput = true;
		  //Hide the axe
		  spriteAxe.setPosition(2000,spriteAxe.getPosition().y);
		}
	    }
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
	    window.close();}
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
	    pause = false;
	    

	  
	  // resetting the game
	  score = 0;
	  timerRemaining = 6;
	  spriteRIP.setPosition(3000,1000);   // remove 
	  // Begone Branches
	  for (int i = 0; i< NUM_BRANCHES;i++){
	    branchPositions[i] = side::NONE;
	  }
	  // move player into position
	  spritePlayer.setPosition(325,550);
	  acceptInput=true;

	  }

	  //wrapping player controls to accept inputs
	  if(acceptInput){
	    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		playerSide = side::RIGHT;
		score++;

		//add to the amount of time remaining
		timerRemaining +=(2/score) + .15;
		spriteAxe.setPosition(AXE_POSITION_RIGHT,
				      spriteAxe.getPosition().y);
		spritePlayer.setPosition(820,550);

		//Updating branches
		updateBranches(score);

		//setting the flying log to the left of the screen
		spriteLog.setPosition(580,600);
		logSpeedX = -5000;
		logActive = true;

		acceptInput = false;

		//play a chop sound
		chop.play();
		
	    }
	  
	      // handling left cursor key
	      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
		//Set player to the left
		playerSide =side::LEFT;
		score++;

		//add time remaining
		timerRemaining += (2/score) + .15;
		spriteAxe.setPosition(AXE_POSITION_LEFT,
				      spriteAxe.getPosition().y);
		spritePlayer.setPosition(325,550);
		//Update branches
		updateBranches(score);

		//set flying log
		spriteLog.setPosition(430,600);
		logSpeedX= 5000;
		logActive =true;
		acceptInput = false;

		// play chop sound
		chop.play();
	      }
	  }
	  
	  if(!pause){
		//Measure time
		sf::Time dt = clock.restart();


	      
		// time remaining Bar
		timerRemaining -=dt.asSeconds();

		// adjust up the time bar
		timeBar.setSize(sf::Vector2f( timeBarWidthPerSecond *
					  timerRemaining, timeBarHeight));
		if (timerRemaining <= 0.0f)
		  {
		    pause = true;
		    messageText.setString("Out of time!!");
		    //reposition the text based on its new size
		    sf::FloatRect textRect = messageText.getLocalBounds();
		    messageText.setOrigin(textRect.left +
					   textRect.width/2.0f,
					   textRect.top +
					   textRect.height/2.0f);
		    messageText.setPosition(1920/2.0f, 600/2.0f);

		    outOfTime.play();
		  }
	     
		  


		
		//Setup the bee
		if(!beeActive){
		  //how fast is the bee
		  srand((int)time(0));
		  beeSpeed = (rand()%200) + 200;
		  
		  //how high is the bee
		  srand((int)time(0)*10);
		  float height=(rand()%500) +500;
		  spriteBee.setPosition(1100,height);
		  beeActive = true;
		}
		 else { //move the bee
		  spriteBee.setPosition(
		  spriteBee.getPosition().x-
		    (beeSpeed *dt.asSeconds()),spriteBee.getPosition().y);
		
		if (spriteBee.getPosition().x< -100){
		  beeActive = false;
		}
		 }
		// manage the clouds

		for ( int i ; i < NUM_OF_CLOUDS; i++){
		if(cloudActive[i]){
		  srand((int)time(0)*10);
		  cloudSpeed[i] = (rand()%200);

		//How high is the cloud
		  srand((int)time(0)*10);
		  float height= (rand()%150);
		  clouds[i].setPosition(-200, height);
		  cloudActive[i] = true;
		  } 
                  else
                {   
                    clouds[i].setPosition(     
                    clouds[i].getPosition().x +      
                    (cloudSpeed[i] * dt.asSeconds()),     
                    clouds[i].getPosition().y);    

                  // Has the cloud reached the right hand edge of the screen?   
                         if (clouds[i].getPosition().x > 1250)   
                          {     
                   // Set it up ready to be a whole new cloud next frame     
                   cloudActive[i] = false;   
  	           	      }
		}
		}
	
	  

		//update the score text         
		SCORE_LAST_DRAWN++;
		
		if( SCORE_LAST_DRAWN = 100)
		  {
     
		
		std::stringstream ss;
		ss<< "Score = " << score;
		scoreText.setString(ss.str());
		SCORE_LAST_DRAWN = 0;
		  }
		
	  
	  //update branches sprite
	  for (int i = 0; i < NUM_BRANCHES;i++)
	    {
	      float height = i*150;
	      if (branchPositions[i] == side::LEFT)
		{
		  
		  //move sprite to left
		  branches[i].setPosition(300, height);
		  //flip sprite
		  branches[i].setRotation(180);
		  branches[i].setOrigin(220,20);
		}
	      else if ( branchPositions[i]== side::RIGHT)
		{
		  // move the sprite right
		  branches[i].setPosition(1000, height);
		  // set sprite back to normal
		  branches[i].setRotation(0);
		  branches[i].setOrigin(220,20);
		}
	      else
		{
		  branches[i].setPosition(3000,height);
		}
	    }
	  	//Handle the flying log
	 if (logActive)
		  {
		    spriteLog.setPosition(
					  spriteLog.getPosition().x
					  +(logSpeedX * dt.asSeconds()),
					  spriteLog.getPosition().y +
					  (logSpeedY * dt.asSeconds()));
		    // log reached the right hand edge?
		    if (spriteLog.getPosition().x < -100||
			spriteLog.getPosition().x >2000)
		      {
			//set it up to be a whole new frame
			logActive =false;
			spriteLog.setPosition(500,600);
		      }
		  }
	 if (branchPositions[5] == playerSide)
	   {
	     //
	     pause = true;
	     acceptInput = false;

	     //Draw the gravestone
	     if(playerSide==side::LEFT)
	       {
		 spriteRIP.setPosition(300,600);
	       }
	     else
	       {
	       spriteRIP.setPosition(800,600);
	       }
	     

	     //hide the player
	     spritePlayer.setPosition(2000,660);

	     //change the text message
	     messageText.setString("SQUISED MOFO");

	     //Center it on the screen
	     sf:: FloatRect textrect =messageText.getLocalBounds();

	     messageText.setOrigin(textRect.left +
				   textRect.width/2.0f,
				   textRect.top + textRect.height/2.0f);
	     messageText.setPosition(1280/2.0f,
				     800/2.0f);
	     death.play();
	   }
	      
	     

	  
		 
		//clear from last frame
		window.clear();
		// Draw our game scene here
		window.draw(spriteBackground);
		
	        for (int i = 0 ; i < NUM_OF_CLOUDS ; i++)
		  {
		    window.draw(clouds[i]);

		  }

		// draw player
			window.draw(spritePlayer);
		//draw axe
		window.draw(spriteAxe);

		//draw log
		window.draw(spriteLog);
		
		
                //draw branches
		for (int i =0; i< NUM_BRANCHES;i++)
		  {
		    window.draw(branches[i]);
		  }
		
	       	window.draw(spriteTree);

		//draw tombstone
		window.draw(spriteRIP);
		window.draw(spriteBee);

		window.draw(scoreText);
		window.draw(timeBar);
		if (pause)
		  {
		    window.draw(messageText);
		  }
		// Show eveything we just drew
		window.display();
		
	}
	}


	return 0;
}


// branch function
void updateBranches(int seed)
{
  //Move all the branches down one place
  for (int j = NUM_BRANCHES -1; j > 0; j--)
    {
      branchPositions[j] = branchPositions[j-1];
    }
  // spawn new branch
  srand((int)time(0) + seed);
  int r = (rand()%5);
  switch (r)
    {
    case 0:
      branchPositions[0] =side::LEFT;
      break;
    case 1:
	branchPositions[0] = side::RIGHT;
      break;
    default:
	branchPositions[0] = side::NONE;
      break;
    }
}
