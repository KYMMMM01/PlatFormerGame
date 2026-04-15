#include "RopeGameMode.h"

#include "RopeCharacter.h"
#include "RopePlayerController.h"

ARopeGameMode::ARopeGameMode()
{
	DefaultPawnClass = ARopeCharacter::StaticClass();
	PlayerControllerClass = ARopePlayerController::StaticClass();
}
