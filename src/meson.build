inc = include_directories(
    '.',
    'core',
    'engine',
    'gfx',
    'characters',
    'backend',
    'backend/sdl',
)

common_sources = files(
    'bagman.cpp',

    # Engine
    'engine/AnimatedObject.cpp',
    'engine/AnimatedSprite.cpp',
    'engine/DirectoryBase.cpp',
    'engine/FadeEvent.cpp',
    'engine/FadeInEvent.cpp',
    'engine/FadeOutEvent.cpp',
    'engine/FrameCouple.cpp',
    'engine/GameContext.cpp',
    'engine/GfxFrame.cpp',
    'engine/GfxFrameSet.cpp',
    'engine/GfxObject.cpp',
    'engine/GfxObjectLayer.cpp',
    'engine/GfxPalette.cpp',
    'engine/JoystickHandler.cpp',
    'engine/Locatable.cpp',
    'engine/MenuScreen.cpp',
    'engine/MPDomain.cpp',
    'engine/MPLevel.cpp',
    'engine/OptionsBase.cpp',
    'engine/PlayerControls.cpp',
    'engine/RandomEvent.cpp',
    'engine/SoundSet.cpp',
    'engine/TileGrid.cpp',

    # Graphics
    'gfx/ColorPalette.cpp',
    'gfx/Drawable.cpp',
    'gfx/FontLetter.cpp',
    'gfx/Fonts.cpp',
    'gfx/ImageFrame.cpp',
    'gfx/SdlColorPalette.cpp',
    'gfx/SDLRectangle.cpp',

    # Characters
    'characters/Character.cpp',
    'characters/Elevator.cpp',
    'characters/Guard.cpp',
    'characters/HumanCharacter.cpp',
    'characters/Player.cpp',
    'characters/Wagon.cpp',

    # Core
    'core/Abortable.cpp',
    'core/Cross.cpp',
    'core/dernc.cpp',
    'core/FunctionId.cpp',
    'core/GsTime.cpp',
    'core/MemoryEntry.cpp',
    'core/MemoryEntryMap.cpp',
    'core/MyAssert.cpp',
    'core/MyFile.cpp',
    'core/MyString.cpp',
    'core/RandomNumber.cpp',
)

platform_sources = []

if host_machine.system() in ['linux', 'windows']

    backend_sources = files(
		'backend/sdl/ImageUtil.cpp',
		'backend/sdl/SoundPlay.cpp',
		'backend/sdl/FunctionTimer.cpp',
		'backend/sdl/MicrosecsClock.cpp',
	)

else

	error('Unsupported platform: ' + host_machine.system())
	
endif

executable(
    'bagman',
    common_sources + backend_sources,
    include_directories : inc,
    dependencies : [
        sdl,
        sdl_mixer,
    ],
    install : true
)
