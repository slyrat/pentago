new winningSet[5]

GetWinningSet()
{
	return winningSet
}
// returns either 0 for no win
// or 1, or 2 for either player 1/2 wins
// or -1 for multi-player win / tie
// lastTurnedSide and lastPlacedSide are numbers indicating sides
CheckForWin(cube[54], lastTurnedSide, lastPlacedSide)
{
	new isWin = false
	isWin = CheckForWinStartingFromSide(cube, lastTurnedSide)
	if (isWin)
	{
		return true
	}
	if (lastTurnedSide != lastPlacedSide)
	{
		return CheckForWinStartingFromSide(cube, lastPlacedSide)
	}
	
	return false
}

CheckForWinStartingFromSide(cube[54], side)
{
	new isWin = false
	isWin = CheckHorizontals(cube, side)
	if (isWin)
	{
		return isWin
	}
	// verticals
	isWin = CheckVerticals(cube, side)
	if (isWin)
	{
		return isWin
	}
	
	// diagonals go here
	isWin = CheckDiagonals(cube, side)
	return isWin
}

new const HORIZONTALS_ONE[3][12] = [[18,19,20,00,01,02,27,28,29,09,10,11],
									[21,22,23,03,04,05,30,31,32,12,13,14],
									[24,25,26,06,07,08,33,34,35,15,16,17]]
new const HORIZONTALS_TWO[3][12] = [[45,46,47,33,30,27,44,43,42,20,23,26],
									[48,49,50,34,31,28,41,40,39,19,22,25],
									[51,52,53,35,32,29,38,37,36,18,21,24]]
new const VERTICALS_ONE[3][12] = [[00,03,06,45,48,51,17,14,11,36,39,42],
								  [01,04,07,46,49,52,16,13,10,37,40,43],
								  [02,05,08,47,50,53,15,12,09,38,41,44]]

new const DIAGONALS[][][] = 	[[[20,42],[04,-1],[23,-1],[43,-1]], //0
								[[42,-1],[05,-1],[03,-1],[44,-1]],  //1
								[[43,-1],[30,-1],[04,-1],[44,27]], 	//2
								[[20,-1],[07,-1],[26,-1],[01,-1]],	//3
								[[00,-1],[08,-1],[06,-1],[02,-1]],	//4
								[[01,-1],[33,-1],[07,-1],[27,-1]],	//5
								[[23,-1],[46,-1],[26,45],[04,-1]],	//6
								[[03,-1],[47,-1],[45,-1],[05,-1]],	//7
								[[04,-1],[47,33],[46,-1],[30,-1]],	//8								
								[[29,38],[13,-1],[32,-1],[37,-1]],	//9
								[[38,-1],[14,-1],[12,-1],[36,-1]],	//10
								[[37,-1],[21,-1],[13,-1],[36,18]],	//11
								[[29,-1],[16,-1],[35,-1],[10,-1]],	//12
								[[09,-1],[17,-1],[15,-1],[11,-1]],	//13
								[[10,-1],[24,-1],[16,-1],[18,-1]],	//14
								[[32,-1],[52,-1],[35,53],[13,-1]],	//15
								[[12,-1],[51,-1],[53,-1],[14,-1]],	//16
								[[13,-1],[24,51],[52,-1],[21,-1]],	//17								
								[[11,36],[22,-1],[14,-1],[39,-1]],	//18
								[[36,-1],[23,-1],[21,-1],[42,-1]],	//19
								[[39,-1],[03,-1],[22,-1],[42,00]],	//20
								[[11,-1],[25,-1],[17,-1],[19,-1]],	//21
								[[18,-1],[26,-1],[24,-1],[20,-1]],	//22
								[[19,-1],[06,-1],[25,-1],[00,-1]],	//23
								[[14,-1],[48,-1],[17,51],[22,-1]],	//24
								[[21,-1],[45,-1],[51,-1],[23,-1]],	//25
								[[22,-1],[45,06],[48,-1],[03,-1]],	//26								
								[[44,02],[31,-1],[05,-1],[41,-1]],	//27
								[[44,-1],[32,-1],[30,-1],[38,-1]],	//28
								[[41,-1],[12,-1],[31,-1],[38,09]],	//29
								[[02,-1],[34,-1],[08,-1],[28,-1]],	//30
								[[27,-1],[35,-1],[33,-1],[29,-1]],	//31
								[[28,-1],[15,-1],[34,-1],[09,-1]],	//32
								[[05,-1],[50,-1],[47,08],[31,-1]],	//33
								[[30,-1],[53,-1],[47,-1],[32,-1]],	//34
								[[31,-1],[15,53],[50,-1],[12,-1]],	//35								
								[[18,11],[40,-1],[19,-1],[10,-1]],	//36
								[[11,-1],[41,-1],[39,-1],[09,-1]],	//37
								[[10,-1],[28,-1],[40,-1],[29,09]],	//38
								[[18,-1],[43,-1],[20,-1],[37,-1]],	//39
								[[36,-1],[44,-1],[42,-1],[38,-1]],	//40
								[[37,-1],[27,-1],[43,-1],[29,-1]],	//41
								[[19,-1],[01,-1],[20,00],[40,-1]],	//42
								[[39,-1],[02,-1],[00,-1],[41,-1]],	//43
								[[40,-1],[27,02],[01,-1],[28,-1]],	//44								
								[[26,06],[49,-1],[25,-1],[07,-1]],	//45
								[[06,-1],[50,-1],[48,-1],[08,-1]],	//46
								[[07,-1],[34,-1],[49,-1],[08,33]],	//47
								[[26,-1],[52,-1],[24,-1],[46,-1]],	//48
								[[45,-1],[53,-1],[51,-1],[47,-1]],	//49
								[[46,-1],[35,-1],[52,-1],[33,-1]],	//50
								[[25,-1],[16,-1],[24,17],[49,-1]],	//51
								[[48,-1],[15,-1],[17,-1],[50,-1]],	//52
								[[49,-1],[35,15],[16,-1],[34,-1]]]	//53	

CheckDiagonals(cube[54], side)
{
	new start = side * 9
	new index
	new color = 0
	new sum = 0
	for(index = 0; index < 9; index++)
	{
		new dir = 0
		color = cube[start + index]
		if (color != 0)
		{
			winningSet[sum] = start + index
			sum = 1
			
			for(dir = 0; dir < 4; dir++)
			{
				sum = GetSumOfDiag(color, cube, index + start, index + start, dir, sum)
				if (sum >= 5)
				{
					return true
				}
				if (dir == 1)
				{
					sum = 1 // reset for left / right directions
				}
			}
		}
	}
	
	return false
}

// sums matching colors for a given direction
GetSumOfDiag(startColor, cube[54], prevIndex, index, dir, sum)
{
	if (sum >= 5)
	{
		return sum
	}
	
	new left
	new right
	new tempLeft = 0
	new tempRight = 0
	
	// first find next direction based on current index / prevIndex
	if (prevIndex != index)
	{
		new i = 0
		for(i = 0; i < 4; i++)
		{
			if (DIAGONALS[index][i][0] == prevIndex ||
				DIAGONALS[index][i][1] == prevIndex)
			{
				break
			}
		}
		if (i == 0 ||
			i == 2)
		{
			i++
		}
		else
		{
			i--
		}
		
		dir = i
	}
	
	
	left = DIAGONALS[index][dir][0]
	right = DIAGONALS[index][dir][1]
	
	if (startColor == cube[left])
	{
		winningSet[sum] = left
		sum++		
		tempLeft = GetSumOfDiag(startColor, cube, index, left, dir, sum)
		if (tempLeft >= 5)
		{
			return tempLeft
		}
	}
	if (right > 0 &&
		sum < 5 &&
		cube[right] == startColor)
	{
		if (startColor == cube[left])
		{
			winningSet[sum-1] = right
		}
		else
		{
			winningSet[sum] = right
			sum++
		}
				
		tempRight = GetSumOfDiag(startColor, cube, index, right, dir, sum)
		if (tempRight >= 5)
		{
			return tempRight
		}
	}
	if (tempLeft > 0 ||
		tempRight > 0)
	{
		if (tempLeft >= tempRight)
		{
			return tempLeft
		}
		else
		{
			return tempRight
		}
	}
	else
	{
		return sum
	}
}

CheckVerticals(cube[54], side)
{
	new startIndex = 0
	new foundVerticalWin = false
	new setToUse[3][12]
	// simple cases
	// set starting indexes
	if (side == 0)
	{
		startIndex = 0
		setToUse = VERTICALS_ONE
	}
	if (side == 5)
	{
		startIndex = 3
		setToUse = VERTICALS_ONE
	}
	if (side == 1)
	{
		startIndex = 6
		setToUse = VERTICALS_ONE
	}
	if (side == 4)
	{
		startIndex = 9
		setToUse = VERTICALS_ONE
	}
	
	// vertical for 3 / 2 is horizontal for 4 / 5
	if (side == 3)
	{
		startIndex = 3
		setToUse = HORIZONTALS_TWO
	}
	if (side == 2)
	{
		startIndex = 9
		setToUse = HORIZONTALS_TWO
	}
	
	new i = 0
	for (i = 0; i < 3; i++)
	{
		foundVerticalWin = CheckBasedOnGivenSet(startIndex, cube, setToUse[i])
		
		if (foundVerticalWin)
		{
			return foundVerticalWin
		}
	}
	
	return foundVerticalWin
}

CheckHorizontals(cube[54], side)
{
	new startIndex = 0
	new foundHorizontalWin = false
	
	// simple cases
	// set starting indexes
	if (side == 0)
	{
		startIndex = 3
	}
	if (side == 1)
	{
		startIndex = 9
	}
	if (side == 2)
	{
		startIndex = 0
	}
	if (side == 3)
	{
		startIndex = 6
	}
	if (side == 4)
	{
		startIndex = 6
	}
	if (side == 5)
	{
		startIndex = 0
	}
	new setToUse[3][12]
	if (side <= 3)
	{
		setToUse = HORIZONTALS_ONE
	}
	else
	{
		setToUse = HORIZONTALS_TWO
	}
	
	new i = 0
	for (i = 0; i < 3; i++)
	{
		foundHorizontalWin = CheckBasedOnGivenSet(startIndex, cube, setToUse[i])
		
		if (foundHorizontalWin)
		{
			return foundHorizontalWin
		}
	}
	
	return foundHorizontalWin
}

CheckBasedOnGivenSet(startIndex, cube[54], lineSet[12])
{
	new i = startIndex
	new initialStart = startIndex
	new foundCol = 0
	new numberInRow = 0
	
	for(i = initialStart; i < startIndex + 3;)
	{
		numberInRow = 0
		foundCol = cube[lineSet[i]]
		if (foundCol != 0)
		{
			// marked, so search left / right
			
			// go right
			while(cube[lineSet[i]] == foundCol && numberInRow < 5)
			{
				winningSet[numberInRow] = lineSet[i]
				numberInRow += 1
				i++
				if (i == 12)
				{
					i = 0
				}
			}
			if (numberInRow < 5)
			{
				// check left of start
				i = startIndex - 1
				if (i < 0)
				{
					i = 11
				}
			
				while(cube[lineSet[i]] == foundCol && numberInRow < 5)
				{
					winningSet[numberInRow] = lineSet[i]
					numberInRow += 1
					i--
					if (i < 0)
					{
						i = 11
					}
				}
			}
			
			if (numberInRow >= 5)
			{
				return true
			}
		}
		initialStart++
		i = initialStart
	}
	
	return false
}