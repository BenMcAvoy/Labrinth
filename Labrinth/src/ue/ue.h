#pragma once

#include "math.h"
#include "containers.h"
#include "strings.h"

#include "uobject.h"
#include "aactor.h"

#include "apawn.h"
#include "scenecomponent.h"
#include "acharacter.h"
#include "skeleton.h"
#include "aplayercameramanager.h"
#include "aplayercontroller.h"
#include "uplayer.h"
#include "ulocalplayer.h"
#include "ugameinstance.h"
#include "aplayerstate.h"
#include "agamestatebase.h"
#include "gamecomponents.h"
#include "items.h"
#include "uworld.h"

namespace Labrinth::UE {
	inline void Init() {
		FNamePool::Get();
		UWorld::Get();
	}
}
