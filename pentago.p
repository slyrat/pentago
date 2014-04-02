/*
This is initial version of pentago, no check for game finish yet, but otherwise seems to work
* still can use some good improvements
*/

#include <futurocube>
#include <WinCheck>

#define   ACC_TRESHOLD  150
#define   SPEED_STEP    150
#define   HALF_SPEED	75
#define	  MAX_PLAYERS	6

new motion
new icon[]=[ICON_MAGIC1,ICON_MAGIC2,2,5,RED,RED,RED,RED,RED,RED,RED,0,0,''stone_placed'',''soko_win'']
new palette[MAX_PLAYERS] = [0]
new colors[]=[cBLUE,cGREEN,cORANGE,WHITE,cRED,cPURPLE]
//note that palette is filled from index 1, index zero has color 0x00000000
new gameState
new const WaitingForPiecePlacement = 1
new const WaitingForTwist = 2
new const WaitingForConfirmation = 3
new const GameWon = 4

new flip = false
new accData[3]    = [0]
new oldAccData[3] = [0]
new absAccData[3] = [0]

main()
{
	ICON(icon)
	RegAllSideTaps()
	SetDoubleTapLength(400)
	RegMotion(TAP_DOUBLE)
	SetIntensity(256)
	SetupPlayers()
	new j=0
	SetColor(palette[j])
	new virtualCanvas[54]
	new waitingCanvas[54]
	new tempCanvas[54]
	new prePiecePlacementCanvas[54]
	new resetCount = 0
	CanvasToArray(virtualCanvas)
	new lastTappedSide
	new placedSide
	new turnedSide
	gameState = WaitingForPiecePlacement
	new showOnlyNew = false
	new motionDir = 1
	
	for (;;)
	{
		motion=Motion()
		
		if (motion)
		{
			lastTappedSide = eTapSide();
			
			if (eTapToTop() == 1 && gameState == WaitingForPiecePlacement)
			{	
				ClearCanvas()
				SetIntensity(256)
				DrawArray(virtualCanvas)
				tempCanvas = virtualCanvas
				cellcopy(prePiecePlacementCanvas, tempCanvas)
				CanvasToArray(virtualCanvas)
				
				new col = ReadCanvas(GetCursor())
				if (col == 0) // no collision, so draw
				{
					Play("drip") // acknowledge to user
					placedSide = lastTappedSide
					DrawPoint(GetCursor())
					CanvasToArray(virtualCanvas)
					gameState = WaitingForTwist
				}
				else
				{
					// collision so tell user
					Vibrate(300)
				}
				
				AckMotion()
				continue
			}
		  
			if (gameState == WaitingForTwist ||
				(gameState == WaitingForConfirmation && _is(motion, TAP_DOUBLE)))
			{
				turnedSide = lastTappedSide
				if (_is(motion, TAP_DOUBLE))
				{
					// previous tap needs to be undone
					showOnlyNew = false
					
					// reset variables
					tempCanvas = virtualCanvas
					cellcopy(waitingCanvas, tempCanvas)
					// redraw previous state
					Draw(virtualCanvas)

					Play("clickhigh")
					motionDir = 1
				}
				else
				{
					Play("click")
					motionDir = -1
				}
				
				// copy current to waiting then manipulate the waiting
				tempCanvas = virtualCanvas
				cellcopy(waitingCanvas,tempCanvas)
				CanvasToArray(virtualCanvas)
				
				// rotate the given side left side tap
				waitingCanvas = RotateSide(waitingCanvas, turnedSide, motionDir);		
				
				gameState = WaitingForConfirmation
				
				AckMotion()
				continue
			}
		  
			if (gameState == WaitingForConfirmation)
			{
				if (eTapToTop() == 1)
				{
					// copy wait to actual canvas
					tempCanvas = waitingCanvas
					cellcopy(virtualCanvas,tempCanvas)
					Draw(virtualCanvas)
					if (CheckForWin(virtualCanvas, turnedSide, placedSide) == true)
					{
						gameState = GameWon
						Play("sfx_win1")
						showOnlyNew = false
						AckMotion()
						continue
					}
					// finally change colour if confirmed
					
					switch(j)
					{
						case 0:
						{
							Play("bell1")
						}
						case 1:
						{
							Play("bell2")
						}
						case 2:
						{
							Play("beep")
						}
						default:
						{
							Play("beep")
						}
					}
					j++
					if (j+1>sizeof(palette)) 
					{
						j=0
					}
					else
					{
						if (palette[j] == 0)
						{
							j = 0
						}
					}
					SetColor(palette[j])
					// play sound for player turn over
					// having switched turn, set back to piece placement
					gameState = WaitingForPiecePlacement
					showOnlyNew = false
				}
				if (eTapToBot() == 1)
				{
					Play("wrong_move")
					// reverse the turn and state
					gameState = WaitingForTwist
					showOnlyNew = false
					
					// reset variables
					tempCanvas = virtualCanvas
					cellcopy(waitingCanvas, tempCanvas)
					// redraw previous state
					Draw(virtualCanvas)
				}
				
				AckMotion()
				continue
			}
			if (gameState == GameWon)
			{
				resetCount++
				if (resetCount > 1)
				{
					resetCount = 0
					new y
					for(y = 0; y < 54; y++)
					{
						virtualCanvas[y] = 0
					}
					Draw(virtualCanvas)
					gameState = WaitingForPiecePlacement
					j = 0 // so player reset works correctly
					SetupPlayers()
					showOnlyNew = false
					SetColor(palette[j])
				}
				AckMotion()
				continue
			}
		}
		else 
		{
			if (gameState != WaitingForPiecePlacement &&
				gameState != GameWon)
			{
				gameState = HandleAccData(virtualCanvas, prePiecePlacementCanvas)
			}
			
			if (gameState == WaitingForPiecePlacement)
			{
				ClearCanvas()
				DrawArray(virtualCanvas)
				DrawFlicker(GetCursor())
			}
			if (gameState == WaitingForConfirmation)
			{
				if (showOnlyNew == false)
				{
					Draw(virtualCanvas)
					showOnlyNew = true
					Delay(SPEED_STEP)
				}
				
				Draw(waitingCanvas)
			}
		}
			
		if (gameState == GameWon)
		{
			new x
			new winSet[5] = [0]
			winSet = GetWinningSet()
			
			// set colour based on what is in one of the winning set
			if (showOnlyNew == false)
			{
				CanvasToArray(virtualCanvas)
				
				new winCol = virtualCanvas[winSet[0]]
				if(winCol != 0)
				{
					printf("%d\n", winCol)
					SetColor(winCol)
				}
				showOnlyNew = true
			}
			
			for(x = 0; x < 5; x++)
			{
				DrawFlicker(winSet[x], 20, FLICK_RAZ)
			}
		}
		
		if (gameState != WaitingForConfirmation)
		{
			PrintCanvas();
		}
		
		Sleep();
	}
} 

SetupPlayers()
{
	new players[MAX_PLAYERS] = [0]
	new i
	new lastTappedSide
	new motionAcked = false
	ClearCanvas()
	for(i = 0; i < 6; i++)
	{
		SetColor(colors[i])
		DrawSide(i)
	}
	// reset palette
	for(i = 0; i < MAX_PLAYERS; i++)
	{
		palette[i] = 0
	}
	
	PrintCanvas()
	
	new chosenColour
	for(;;)
	{
		motion=Motion()
		if(motion)
		{
			if((eTapToBot() == 1) == false)
			{
				lastTappedSide = eTapSide()
				
				chosenColour = ReadCanvas(9*lastTappedSide)
				for(i = 0; i < MAX_PLAYERS; i++)
				{
					if(players[i] == chosenColour)
					{
						players[i] = 0 // clear
						SetColor(0)
						DrawSide(lastTappedSide)
						SetColor(chosenColour)
						DrawSide(lastTappedSide)
						AckMotion()
						Play("click")
						motionAcked = true
						break
					}
				}
				if (motionAcked == false)
				{
					for(i = 0; i < MAX_PLAYERS; i++)
					{
						if(players[i] == 0)
						{
							// set next available player
							players[i] = chosenColour
							SetColor(0)
							DrawSide(lastTappedSide)
							SetColor(chosenColour)
							DrawSquare(9*lastTappedSide + 4)
							Play("clickhigh")
							AckMotion()
							break
						}
					}
				}
			}
			else
			{
				new j = 0
				for(i = 0; i < MAX_PLAYERS; i++)
				{
					if (players[i] != 0)
					{
						palette[j] = players[i]
						j++
					}
				}
				if (j >= 2)
				{
					ClearCanvas()
					return 
				}	
				AckMotion()
			}
		
			motionAcked = false
		}
		
		PrintCanvas()
	}
}


// Handle shake which means undo move
HandleAccData(cube[54], oldCube[54])
{
	// keep track of acc data for detection of shaking
	/* substruct current from previous values, the easiest way to eliminate gravity bias */
	if (flip)
	{
		ReadAcc(accData, 6)
		absAccData[0] = abs(accData[0] - oldAccData[0])
		absAccData[1] = abs(accData[1] - oldAccData[1])
		absAccData[2] = abs(accData[2] - oldAccData[2])
	} else {
		ReadAcc(oldAccData, 0)
		absAccData[0] = abs(oldAccData[0] - accData[0])
		absAccData[1] = abs(oldAccData[1] - accData[1])
		absAccData[2] = abs(oldAccData[2] - accData[2])
	}
	flip = !flip
	
	if (absAccData[1] > 200 && absAccData[2] > 200 && absAccData[0] > 200)
	{
		cube = oldCube
		Draw(oldCube)
		Play("wrong_move")
		gameState = WaitingForPiecePlacement
	}
	
	return gameState
}

Draw(side[54])
{
  ClearCanvas()
  DrawArray(side)
  PrintCanvas()
}

new const _t_side_rot[8]=[3,6,7,8,5,2,1,0]

MakeShift(belt[],currentCube[],length,dir=1)
{
  new temp
  new i
  if (dir==1)
  {
   temp=currentCube[belt[0]]
   for (i=0;i<(length-1);i++)
    currentCube[belt[i]]=currentCube[belt[i+1]]
   currentCube[belt[length-1]]=temp
  }
  else
  {
   temp=currentCube[belt[length-1]]
   for (i=length-1;i>0;i--)
    currentCube[belt[i]]=currentCube[belt[i-1]]
   currentCube[belt[0]]=temp
  }
}

// direction, 1 for right, -1 for left
RotateSide(side[54], sideNumber, direction)
{
	new bbelt[8]
	new i
	for (i=0;i<8;i++) bbelt[i]=sideNumber*9+_t_side_rot[i]
	
	MakeShift(bbelt,side,8,direction)
	Draw(side)
	MakeShift(bbelt,side,8,direction)
	Draw(side)
	return side;
}