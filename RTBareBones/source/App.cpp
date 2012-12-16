/*
 *  App.cpp
 *  Created by Seth Robinson on 3/6/09.
 *  For license info, check the license.txt file that should have come with this.
 *
 */
#include "PlatformPrecomp.h"
#include "App.h"

#include "Entity/CustomInputComponent.h" //used for the back button (android)
#include "Entity/FocusInputComponent.h" //needed to let the input component see input messages
#include "Entity/ArcadeInputComponent.h" 
//#include "util/TextScanner.h"

#include "game.h"

MessageManager g_messageManager;
MessageManager * GetMessageManager() {return &g_messageManager;}

FileManager g_fileManager;
FileManager * GetFileManager() {return &g_fileManager;}


#ifdef __APPLE__

#if TARGET_OS_IPHONE == 1
        //it's an iPhone or iPad
        //#include "Audio/AudioManagerOS.h"
        //AudioManagerOS g_audioManager;
        #include "Audio/AudioManagerDenshion.h"
        
        AudioManagerDenshion g_audioManager;
#else
        //it's being compiled as a native OSX app
   #include "Audio/AudioManagerFMOD.h"
  AudioManagerFMOD g_audioManager; //dummy with no sound

//in theory, CocosDenshion should work for the Mac builds, but right now it seems to want a big chunk of
//Cocos2d included so I'm not fiddling with it for now

//#include "Audio/AudioManagerDenshion.h"
//AudioManagerDenshion g_audioManager;
#endif
        
#else

#if defined RT_WEBOS || defined RTLINUX
#include "Audio/AudioManagerSDL.h"
AudioManagerSDL g_audioManager; //sound in windows and WebOS
//AudioManager g_audioManager; //to disable sound
#elif defined ANDROID_NDK
#include "Audio/AudioManagerAndroid.h"
AudioManagerAndroid g_audioManager; //sound for android
#elif defined PLATFORM_BBX
#include "Audio/AudioManagerBBX.h"
//AudioManager g_audioManager; //to disable sound
AudioManagerBBX g_audioManager;
#elif defined PLATFORM_FLASH
//AudioManager g_audioManager; //to disable sound
#include "Audio/AudioManagerFlash.h"
AudioManagerFlash *g_audioManager = new AudioManagerFlash;
#else


//in windows
//AudioManager g_audioManager; //to disable sound

#ifdef RT_FLASH_TEST
#include "Audio/AudioManagerFlash.h"
AudioManagerFlash g_audioManager;
#else

#include "Audio/AudioManagerAudiere.h"
AudioManagerAudiere g_audioManager;  //Use Audiere for audio
#endif
//#include "Audio/AudioManagerFMOD.h"
//AudioManagerFMOD g_audioManager; //if we wanted FMOD sound in windows

#endif
#endif

#if defined PLATFORM_FLASH
        AudioManager * GetAudioManager(){return g_audioManager;}
#else
        AudioManager * GetAudioManager(){return &g_audioManager;}
#endif

App *g_pApp = NULL;

BaseApp * GetBaseApp() 
{
	if (!g_pApp)
	{
		g_pApp = new App;
	}
	return g_pApp;
}

App * GetApp() 
{
	assert(g_pApp && "GetBaseApp must be called used first");
	return g_pApp;
}

App::App()
{
	m_bDidPostInit = false;
}

App::~App()
{
}

bool App::Init()
{
	
	if (m_bInitted)	
	{
		return true;
	}
	
	if (!BaseApp::Init()) return false;
	
	if (GetEmulatedPlatformID() == PLATFORM_ID_IOS || GetEmulatedPlatformID() == PLATFORM_ID_WEBOS)
	{
		//SetLockedLandscape( true); //if we don't allow portrait mode for this game
		//SetManualRotationMode(true); //don't use manual, it may be faster (33% on a 3GS) but we want iOS's smooth rotations
	}

	LogMsg("The Save path is %s", GetSavePath().c_str());
	LogMsg("Region string is %s", GetRegionString().c_str());

#ifdef _DEBUG
	LogMsg("Built in debug mode");
#endif
#ifndef C_NO_ZLIB
	//fonts need zlib to decompress.  When porting a new platform I define C_NO_ZLIB and add zlib support later sometimes
	if (!GetFont(FONT_SMALL)->Load("interface/font_trajan.rtfont")) return false;
#endif

	GetBaseApp()->SetFPSVisible(true);
	GetBaseApp()->SetFPSLimit(30);

	/**
	* My init stuff
	*/
	g_game = new game();
	g_game->init();
	return true;
}

void App::Kill()
{
	BaseApp::Kill();
}

void App::OnExitApp(VariantList *pVarList)
{
	LogMsg("Exiting the app");
	OSMessage o;
	o.m_type = OSMessage::MESSAGE_FINISH_APP;
	GetBaseApp()->AddOSMessage(o);
}

#define kFilteringFactor 0.1f
#define C_DELAY_BETWEEN_SHAKES_MS 500

//testing accelerometer readings. To enable the test, search below for "ACCELTEST"
//Note: You'll need to look at the  debug log to see the output. (For android, run PhoneLog.bat from RTBareBones/android)
void App::OnAccel(VariantList *pVList)
{
	
	if ( int(pVList->m_variant[0].GetFloat()) != MESSAGE_TYPE_GUI_ACCELEROMETER) return;

	CL_Vec3f v = pVList->m_variant[1].GetVector3();

	LogMsg("Accel: %s", PrintVector3(v).c_str());

	v.x = v.x * kFilteringFactor + v.x * (1.0f - kFilteringFactor);
	v.y = v.y * kFilteringFactor + v.y * (1.0f - kFilteringFactor);
	v.z = v.z * kFilteringFactor + v.z * (1.0f - kFilteringFactor);

	// Compute values for the three axes of the acceleromater
	float x = v.x - v.x;
	float y = v.y - v.x;
	float z = v.z - v.x;

	//Compute the intensity of the current acceleration 
	if (sqrt(x * x + y * y + z * z) > 2.0f)
	{
		Entity *pEnt = GetEntityRoot()->GetEntityByName("jumble");
		if (pEnt)
		{
			//GetAudioManager()->Play("audio/click.wav");
            VariantList vList(CL_Vec2f(), pEnt);
			pEnt->GetFunction("OnButtonSelected")->sig_function(&vList);
		}
		LogMsg("Shake!");
	}
}


//test for arcade keys.  To enable this test, search for TRACKBALL/ARCADETEST: below and uncomment the stuff under it.
//Note: You'll need to look at the debug log to see the output.  (For android, run PhoneLog.bat from RTBareBones/android)

void App::OnArcadeInput(VariantList *pVList)
{

	int vKey = pVList->Get(0).GetUINT32();
	eVirtualKeyInfo keyInfo = (eVirtualKeyInfo) pVList->Get(1).GetUINT32();
	
	string pressed;


	

	string keyName = "unknown";

	switch (vKey)
	{
		case VIRTUAL_KEY_DIR_LEFT:
			keyName = "Left";
			g_game->moving_dir_ = 0;
			break;

		case VIRTUAL_KEY_DIR_RIGHT:
			keyName = "Right";
			g_game->moving_dir_ = 1;
			break;

		case VIRTUAL_KEY_DIR_UP:
			keyName = "Up";
			g_game->moving_dir_ = 2;
			break;

		case VIRTUAL_KEY_DIR_DOWN:
			keyName = "Down";
			g_game->moving_dir_ = 3;
			break;

		case VIRTUAL_KEY_CONTROL:
			keyName = "Shift";
			g_game->pick_up_item();
			break;

	}

	switch (keyInfo)
	{
		case VIRTUAL_KEY_PRESS:
			pressed = "pressed";
			break;

		case VIRTUAL_KEY_RELEASE:
			pressed = "released";
			g_game->moving_dir_ = -1;
			break;

		default:
			LogMsg("OnArcadeInput> Bad value of %d", keyInfo);
	}
	
	LogMsg("Arcade input: Hit %d (%s) (%s)", vKey, keyName.c_str(), pressed.c_str());
}


void AppInput(VariantList *pVList)
{

	//0 = message type, 1 = parent coordinate offset, 2 is fingerID
	eMessageType msgType = eMessageType( int(pVList->Get(0).GetFloat()));
	CL_Vec2f pt = pVList->Get(1).GetVector2();
	//pt += GetAlignmentOffset(*m_pSize2d, eAlignment(*m_pAlignment));

	
	uint32 fingerID = 0;
	if ( msgType != MESSAGE_TYPE_GUI_CHAR && pVList->Get(2).GetType() == Variant::TYPE_UINT32)
	{
		fingerID = pVList->Get(2).GetUINT32();
	}

	CL_Vec2f vLastTouchPt = GetBaseApp()->GetTouch(fingerID)->GetLastPos();

	switch (msgType)
	{
	case MESSAGE_TYPE_GUI_CLICK_START:
		//LogMsg("Touch start: X: %.2f YL %.2f (Finger %d)", pt.x, pt.y, fingerID);
		break;
	case MESSAGE_TYPE_GUI_CLICK_MOVE:
		//LogMsg("Touch mode: X: %.2f YL %.2f (Finger %d)", pt.x, pt.y, fingerID);
		break;
	case MESSAGE_TYPE_GUI_CLICK_END:
		LogMsg("Touch end: X: %.2f YL %.2f (Finger %d)", pt.x, pt.y, fingerID);
		//menu click
		if (g_game->game_state_ == 0)
		{
			//314/264
			//674/330
			if ((pt.x > 314 && pt.x < 674) &&
				(pt.y > 264 && pt.y < 330))
			{
				//LogMsg("Click on start");
				g_game->game_state_ = 4;
			}
			if ((pt.x > 314 && pt.x < 674) &&
				(pt.y > 422 && pt.y < 486))
			{
				//LogMsg("Click on music");
				if (GetAudioManager()->GetMusicEnabled() == false)
				{
					GetAudioManager()->SetMusicEnabled(true);
					GetAudioManager()->Play("interface/korra.mp3", true, true);
				}
				else
				{
					GetAudioManager()->SetMusicEnabled(false);
				}
			}
		}
		else if (g_game->game_state_ == 3)
		{

			if ((pt.x > 328 && pt.x < 683) &&
				(pt.y > 438 && pt.y < 496))
			{
				//LogMsg("Click on start");
				g_game->game_state_ = 0;
			}
		}
		else if (g_game->game_state_ == 2)
		{
			if ((pt.x > 581 && pt.x < 948) &&
				(pt.y > 384 && pt.y < 447))
			{
				//LogMsg("Click on start");
				g_game->game_state_ = 0;
			}
			if ((pt.x > 149 && pt.x < 507) &&
				(pt.y > 384 && pt.y < 447))
			{
				//LogMsg("Click on start");
				g_game->start_level(g_game->current_level_);
			}
		}
		else if (g_game->game_state_ == 4)
		{
			if ((pt.x > 367 && pt.x < 451) &&
				(pt.y > 40 && pt.y < 109) &&
				g_game->level_ >= 1)
			{
				g_game->start_level(1);
			}
			if ((pt.x > 459 && pt.x < 546) &&
				(pt.y > 88 && pt.y < 164) &&
				g_game->level_ >= 2)
			{
				g_game->start_level(2);
			}
			if ((pt.x > 558 && pt.x < 639) &&
				(pt.y > 130 && pt.y < 210) &&
				g_game->level_ >= 3)
			{
				g_game->start_level(3);
			}
			if ((pt.x > 649 && pt.x < 723) &&
				(pt.y > 175 && pt.y < 256) &&
				g_game->level_ >= 4)
			{
				g_game->start_level(4);
			}
			if ((pt.x > 736 && pt.x < 813) &&
				(pt.y > 224 && pt.y < 296) &&
				g_game->level_ >= 5)
			{
				g_game->start_level(5);
			}
			if ((pt.x > 827 && pt.x < 912) &&
				(pt.y > 262 && pt.y < 342) &&
				g_game->level_ >= 6)
			{
				g_game->start_level(6);
			}
		}
		break;
	}	
}


void App::Update()
{
	
	//game can think here.  The baseApp::Update() will run Update() on all entities, if any are added.  The only one
	//we use in this example is one that is watching for the Back (android) or Escape key to quit that we setup earlier.

	BaseApp::Update();

	if (!m_bDidPostInit)
	{
		//stuff I want loaded during the first "Update"
		m_bDidPostInit = true;
		
		//for android, so the back key (or escape on windows) will quit out of the game
		Entity *pEnt = GetEntityRoot()->AddEntity(new Entity);
		EntityComponent *pComp = pEnt->AddComponent(new CustomInputComponent);
		//tell the component which key has to be hit for it to be activated
		pComp->GetVar("keycode")->Set(uint32(VIRTUAL_KEY_BACK));
		//attach our function so it is called when the back key is hit
		pComp->GetFunction("OnActivated")->sig_function.connect(1, boost::bind(&App::OnExitApp, this, _1));

		//nothing will happen unless we give it input focus
		pEnt->AddComponent(new FocusInputComponent);

		//ACCELTEST:  To test the accelerometer uncomment below: (will print values to the debug output)
		//SetAccelerometerUpdateHz(25); //default is 0, disabled
		//GetBaseApp()->m_sig_accel.connect(1, boost::bind(&App::OnAccel, this, _1));

		//TRACKBALL/ARCADETEST: Uncomment below to see log messages on trackball/key movement input
		pComp = pEnt->AddComponent(new ArcadeInputComponent);
		GetBaseApp()->m_sig_arcade_input.connect(1, boost::bind(&App::OnArcadeInput, this, _1));
	
		//these arrow keys will be triggered by the keyboard, if applicable
		AddKeyBinding(pComp, "Left", VIRTUAL_KEY_DIR_LEFT, VIRTUAL_KEY_DIR_LEFT);
		AddKeyBinding(pComp, "Right", VIRTUAL_KEY_DIR_RIGHT, VIRTUAL_KEY_DIR_RIGHT);
		AddKeyBinding(pComp, "Up", VIRTUAL_KEY_DIR_UP, VIRTUAL_KEY_DIR_UP);
		AddKeyBinding(pComp, "Down", VIRTUAL_KEY_DIR_DOWN, VIRTUAL_KEY_DIR_DOWN);
		AddKeyBinding(pComp, "Shift", VIRTUAL_KEY_CONTROL, VIRTUAL_KEY_CONTROL);
		AddKeyBinding(pComp, "Fire", VIRTUAL_KEY_CONTROL, VIRTUAL_KEY_GAME_FIRE);

		//INPUT TEST - wire up input to some functions to manually handle.  AppInput will use LogMsg to
		//send them to the log.  (Each device has a way to view a debug log in real-time)
		GetBaseApp()->m_sig_input.connect(&AppInput);

		/*
		//file handling test, if TextScanner.h is included at the top..

		TextScanner t;
		t.m_lines.push_back("Testing 123");
		t.m_lines.push_back("Fuck ya'll!");
		t.m_lines.push_back("Whoopsopsop!");

		LogMsg("Saving file...");
		t.SaveFile("temp.txt");


		TextScanner b;
		b.LoadFile("temp.txt");
		b.DumpToLog();
		*/
	}

	//game is thinking.  
	if (g_game->game_state_ == 1) //we update the objects
	{
		g_game->update();
	}
}

void App::Draw()
{
	//Use this to prepare for raw GL calls
	PrepareForGL();
	
	glClearColor(0.5,0.5,0.5,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//after our 2d rect call above, we need to prepare for raw GL again. (it keeps it in ortho mode if we don't for speed)
	PrepareForGL();

	if (g_game->game_state_ == 0 || g_game->game_state_ == 4 || g_game->game_state_ == 2 || g_game->game_state_ == 3) //we draw main menu
	{
		g_game->draw_main_menu();
	}
	else if (g_game->game_state_ == 1)
	{
		//tile_map[][] = [[...],...]
		int tile_map_size = g_game->world_size_;
		int tile_width = 80;
		int tile_height = 40;
		int sel_pos_x = 0;
		int sel_pos_y = 0;
	
		// i goes from zero to 9
		// j goes from 1 to 10... fuck

		
		for (int i = 0; i < tile_map_size; i++)
		{
			int j = 13;
			g_game->spritesheet_.BlitFrame((j * tile_width / 2) + (i * tile_width / 2)+10, (i * tile_height / 2) - (j * tile_height / 2)+GetScreenSizeY()/2,"pared_1");
		}
		for (int j = tile_map_size; j > 0; j--)
		{
			int i = -1;
			g_game->spritesheet_.BlitFrame((j * tile_width / 2) + (i * tile_width / 2)+10, (i * tile_height / 2) - (j * tile_height / 2)+GetScreenSizeY()/2,"pared_2");
		}

		for (int i = 0; i < tile_map_size; i++)
		{
			for (int j = tile_map_size; j > 0; j--)
			{
				g_game->spritesheet_.BlitFrame((j * tile_width / 2) + (i * tile_width / 2)+10, (i * tile_height / 2) - (j * tile_height / 2)+GetScreenSizeY()/2,"floor_default");
				//g_game->spritesheet_.BlitFrame((j * tile_width / 2) + (i * tile_width / 2)+10, (i * tile_height / 2) - (j * tile_height / 2)+GetScreenSizeY()/2,"floor_3");

				//we draw the player
				if (g_game->player_.x_ == i && g_game->player_.y_ == (j-1))
				{
					if (g_game->player_.looking_direction_ == 0) //left
					{
						g_game->spritesheet_.BlitFrame((j * tile_width / 2) + (i * tile_width / 2)+10, (i * tile_height / 2) - (j * tile_height / 2)+GetScreenSizeY()/2,"char_0");
					}
					else if (g_game->player_.looking_direction_ == 1) //right
					{
						g_game->spritesheet_.BlitFrame((j * tile_width / 2) + (i * tile_width / 2)+10, (i * tile_height / 2) - (j * tile_height / 2)+GetScreenSizeY()/2,"char_1");
					}
					else if (g_game->player_.looking_direction_ == 2) //up
					{
						g_game->spritesheet_.BlitFrame((j * tile_width / 2) + (i * tile_width / 2)+10, (i * tile_height / 2) - (j * tile_height / 2)+GetScreenSizeY()/2,"char_2");
					}
					else if (g_game->player_.looking_direction_ == 3) //down
					{
						g_game->spritesheet_.BlitFrame((j * tile_width / 2) + (i * tile_width / 2)+10, (i * tile_height / 2) - (j * tile_height / 2)+GetScreenSizeY()/2,"char_3");
					}
				}


				if (g_game->layer_vision[i][j-1] != 0)
				{
					g_game->draw_vision(i,j);
				}

				if (g_game->layer_enemigos[i][j-1] != 0)
				{
					g_game->draw_enemigo(i,j);
				}

				if (g_game->layer_map[i][j-1] != 0)
				{
					g_game->draw_item(i,j);
				}

				if (g_game->layer_top[i][j-1] != 0)
				{
					g_game->draw_item_top(i,j);
				}

				if (g_game->layer_walls[i][j-1] != 0)
				{
					g_game->draw_walls(i,j);
				}
			}
		}
		for (int i = 0; i < tile_map_size; i++)
		{
			for (int j = tile_map_size; j > 0; j--)
			{
				if (g_game->only_render[i][j-1] != 0)
				{
					g_game->draw_item_only_render(i,j);
				}
			}
		}
	}
	
	//the base handles actually drawing the GUI stuff over everything else, if applicable, which in this case it isn't.
	BaseApp::Draw();
}



void App::OnScreenSizeChange()
{
	BaseApp::OnScreenSizeChange();
}

void App::OnEnterBackground()
{
	//save your game stuff here, as on some devices (Android <cough>) we never get another notification of quitting.
	LogMsg("Entered background");
	BaseApp::OnEnterBackground();
}

void App::OnEnterForeground()
{
	LogMsg("Entered foreground");
	BaseApp::OnEnterForeground();
}

const char * GetAppName() {return "StickThief";}

//the stuff below is for android/webos builds.  Your app needs to be named like this.

//note: these are put into vars like this to be compatible with my command-line parsing stuff that grabs the vars

const char * GetBundlePrefix()
{
	const char * bundlePrefix = "com.rtsoft.";
	return bundlePrefix;
}

const char * GetBundleName()
{
	const char * bundleName = "rtbarebones";
	return bundleName;
}
