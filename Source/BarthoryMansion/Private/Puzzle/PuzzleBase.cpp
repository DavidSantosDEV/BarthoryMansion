// Fropple Studios Copyright


#include "Puzzle/PuzzleBase.h"
#include "Puzzles/PuzzleMaster.h"

void APuzzleBase::OnPuzzleComplete()
{
	if (MyMaster) {
		bIsCompleted = true;
		bIsInteractible = false;
	}
}

void APuzzleBase::SetIsComplete(bool bComplete)
{
	if (MyMaster) {

	}
}

void APuzzleBase::SetMaster(APuzzleMaster* newMaster)
{
	MyMaster = newMaster;
}
