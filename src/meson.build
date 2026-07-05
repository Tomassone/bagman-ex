inc = include_directories(
    '.',
    'engine',
    'gfx',
    'characters',
    'platform',
)

if host_machine.system() == 'linux'
    inc += include_directories('platform/linux')
elif host_machine.system() == 'windows'
    inc += include_directories('platform/windows')
endif

common_sources = files(
    'bagman.cpp',

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

    'gfx/ColorPalette.cpp',
	'gfx/Drawable.cpp',
	'gfx/FontLetter.cpp',
	'gfx/Fonts.cpp',
	'gfx/ImageFrame.cpp',
	'gfx/SdlColorPalette.cpp',
	'gfx/SDLRectangle.cpp',

    'characters/Character.cpp',
	'characters/Elevator.cpp',
	'characters/Guard.cpp',
	'characters/HumanCharacter.cpp',
	'characters/Player.cpp',
	'characters/Wagon.cpp',
	
	'platform/Abortable.cpp',
	'platform/Cross.cpp',
	'platform/dernc.cpp',
	'platform/FunctionId.cpp',
	'platform/GsTime.cpp',
	'platform/ImageUtil.cpp',
	'platform/MemoryEntry.cpp',
	'platform/MemoryEntryMap.cpp',
	'platform/MyAssert.cpp',
	'platform/MyFile.cpp',
	'platform/MyString.cpp',
	'platform/RandomNumber.cpp',
	'platform/SoundPlay.cpp',
)

platform_sources = []

if host_machine.system() == 'linux'

    platform_sources += files(
        'platform/linux/FunctionTimer.cpp',
		'platform/linux/MicrosecsClock.cpp',
    )

elif host_machine.system() == 'windows'

    platform_sources += files(
        'platform/windows/FunctionTimer.cpp',
		'platform/windows/MicrosecsClock.cpp',
    )

else

	error('Unsupported platform: ' + host_machine.system())
	
endif

executable(
    'bagman',
    common_sources + platform_sources,
    include_directories : inc,
    dependencies : [
        sdl,
        sdl_mixer,
    ],
    install : true
)

install_subdir('../resource')
install_subdir('../sound')
