#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>

#include <sstream>
#include <iomanip>

const std::string TITLE = "Pickin' Sticks SFML Version 1";

int main()
{
	sf::Vector2i WindowDimensions(1280, 720);
	sf::RenderWindow Window(sf::VideoMode(WindowDimensions.x, WindowDimensions.y), TITLE);

	Window.setKeyRepeatEnabled(false);

	float frameCounter = 0, switchCounter = 75, frameSpeed = 416;

	sf::Texture pTexture;
	sf::Sprite playerImage;

	sf::Clock clock;

	float curMoveSpeed = 0;
	float maxMoveSpeed = 3.5f;
	int curSticksPicked = 0;
	sf::Music bgMusic;

	if(!bgMusic.openFromFile("Sounds/Music/MusicSound.ogg"))
		std::cout << "OH NA NA NA" << std::endl;
	bgMusic.setVolume(25);
	bgMusic.setLoop(true);

	sf::SoundBuffer buffer;
	sf::Sound soundEffect;

    if (!buffer.loadFromFile("Sounds/Effects/baseball_hit.wav"))
		return EXIT_FAILURE;

	soundEffect.setBuffer(buffer);

	bgMusic.play();

	sf::RectangleShape BlackBar(sf::Vector2f(WindowDimensions.x, WindowDimensions.y/20));
	BlackBar.setFillColor(sf::Color::Black);
	sf::Texture BGT;
	BGT.setRepeated(true);
	if (!BGT.loadFromFile("Textures/Grass.png"));
	sf::RectangleShape BG(sf::Vector2f(WindowDimensions.x,  WindowDimensions.y + WindowDimensions.y/20.0f));
	BG.setTexture(&BGT);

	sf::Font font;
	if (!font.loadFromFile("Fonts/Font.ttf"));
	
	enum Directions
	{
		Down = 0,
		Left = 1,
		Right = 2,
		Up = 3,
	};

	enum Ranks
	{
		Bad = 0,
		Ok = 10,
		Awesome = 20,
		Posh = 30,
		SWANKY = 40,
		Evil = 100
	};

	Ranks curRank = Bad;

	sf::Vector2i source(1, Down); 

	if(!pTexture.loadFromFile("Textures/Character.png"))
		return EXIT_FAILURE;

	playerImage.setTexture(pTexture);
	playerImage.setPosition(640-32, 360-16);
	
	std::srand((unsigned)time(NULL));
	sf::Vector2f RandomPos(std::rand() % WindowDimensions.x - 32, std::rand() %  (WindowDimensions.y - 64) + (WindowDimensions.y/20));

	sf::Texture bTexture;
	sf::Sprite branchImage;

	if(!bTexture.loadFromFile("Textures/Branch.png"))
		return EXIT_FAILURE;

	branchImage.setTexture(bTexture);
	branchImage.setScale(0.06f,0.06f);

	branchImage.setPosition(RandomPos);

    while (Window.isOpen())
    {
        sf::Event Event;

	std::string curSticksPickedString = std::to_string(curSticksPicked);
	std::string curMoveSpeedString = std::to_string(curMoveSpeed);
	std::string curRankString;

	if(playerImage.getPosition().x < branchImage.getPosition().x + branchImage.getGlobalBounds().width &&
			playerImage.getPosition().x + playerImage.getLocalBounds().width  > branchImage.getPosition().x &&
			playerImage.getPosition().y < branchImage.getPosition().y + branchImage.getGlobalBounds().height &&
			playerImage.getPosition().y + playerImage.getLocalBounds().height  > branchImage.getPosition().y)
		{
			sf::Vector2f RandomPos(std::rand() % WindowDimensions.x - 32, std::rand() %  (WindowDimensions.y - 128) + (WindowDimensions.y/20));
			branchImage.setPosition(RandomPos);
			curSticksPicked++;
			soundEffect.play();
		}

	if(curSticksPicked >= 0)
	{
		curRank = Bad;
		curRankString = "..meh";
		curMoveSpeed = 0.1f * maxMoveSpeed;
	}

	if(curSticksPicked >= 10)
	{
		curRank = Ok;
		curRankString = "Not Bad";
		curMoveSpeed = 0.2f * maxMoveSpeed;
	}

	if(curSticksPicked >= 20)
	{
		curRank = Awesome;
		curRankString = "Pretty Good";
		curMoveSpeed = 0.4f * maxMoveSpeed;
	}

	if(curSticksPicked >= 30)
	{
		curRank = Posh;
		curRankString = "Awesome!";
		curMoveSpeed = 0.8f * maxMoveSpeed;
	}

	if(curSticksPicked >= 40)
	{
		curRank = SWANKY;
		curRankString = "ULTRA COMBO!!!";
		curMoveSpeed = maxMoveSpeed;
	}

	if(curSticksPicked >= 100)
	{
		curRank = Evil;
		curRankString = "TROLOLOL!!!!";
		curMoveSpeed = maxMoveSpeed * 2.5;
	}
		
	sf::Text ScoreText;
	ScoreText.setFont(font);
	ScoreText.setString("Score: " + curSticksPickedString);
	ScoreText.setCharacterSize(34);
	ScoreText.setColor(sf::Color::White);
	ScoreText.setStyle(sf::Text::Bold);
	ScoreText.setPosition(0 + WindowDimensions.x / 8.0f, 0);

	sf::Text SpeedText;
	SpeedText.setFont(font);
	SpeedText.setString("Current Speed: " + curMoveSpeedString);
	SpeedText.setCharacterSize(34);
	SpeedText.setColor(sf::Color::White);
	SpeedText.setStyle(sf::Text::Bold);
	SpeedText.setPosition((ScoreText.getPosition().x + ScoreText.getScale().x) + ScoreText.getGlobalBounds().width + 55, 0);

	sf::Text RankText;
	RankText.setFont(font);
	RankText.setString("Current Rank: " + curRankString);
	RankText.setCharacterSize(34);
	RankText.setColor(sf::Color::White);
	RankText.setStyle(sf::Text::Bold);
	RankText.setPosition((SpeedText.getPosition().x + SpeedText.getScale().x) + SpeedText.getGlobalBounds().width + 55, 0);


        while (Window.pollEvent(Event))
        {
            if (Event.type == sf::Event::Closed)
                Window.close();
        }

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			curSticksPicked++;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || 
		   sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			source.y = Down;
		}


		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 
				sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			source.y = Left;
		}

		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || 
				sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			source.y = Right;
		}

		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || 
				sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
		{
			source.y = Up;
		}

		switch (source.y)
		{
			case Down:
				if(playerImage.getPosition().y <= (Window.getSize().y - playerImage.getPosition().y / 32) - 32)
					playerImage.move(0,curMoveSpeed);
				else
				{
					frameCounter = 0;
					source.x = 0;
				}
				break;

			case Left:
				if(playerImage.getPosition().x >= (Window.getSize().x - Window.getSize().x))
					playerImage.move(-curMoveSpeed,0);
				else
				{
					frameCounter = 0;
					source.x = 0;
				}
				break;

			case Right:
				if(playerImage.getPosition().x <= (Window.getSize().x - playerImage.getPosition().x / 32))
					playerImage.move(curMoveSpeed,0);
				else
				{
					frameCounter = 0;
					source.x = 0;
				}
				break;

			case Up:
				if(playerImage.getPosition().y >= ((Window.getSize().y - Window.getSize().y) + WindowDimensions.y/20))
					playerImage.move(0,-curMoveSpeed);
				else
				{
					frameCounter = 0;
					source.x = 0;
				}
				break;

			default:
				playerImage.move(0, 0);
				break;
		}
	
		frameCounter+= frameSpeed * clock.restart().asSeconds();

		if(frameCounter >= switchCounter)
		{
			frameCounter = 0;
			source.x++;
			if(source.x * 32 >= pTexture.getSize().x)
				source.x = 0;
		}
		playerImage.setTextureRect(sf::IntRect(source.x * 32, source.y * 32, 32, 32));

		Window.draw(BG);
		Window.draw(branchImage);
		Window.draw(playerImage);
		Window.draw(BlackBar);

		Window.draw(ScoreText);
		Window.draw(SpeedText);
		Window.draw(RankText);

		Window.display();
    }

	return EXIT_SUCCESS;
}

