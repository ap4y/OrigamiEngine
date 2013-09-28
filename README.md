Lightweight iOS and OSX audio engine with opus, flac, cue, mp3, m4a, m3u support.

Supported formats
-------

  * Audio sources: http and local files
  * Audio files: `opus`, `flac`, `mp3`, `m4a`, `wav` and other from CoreAudio
  * Playlists: `cue`, `m3u`


Features
-------

- Ligthweight: 300kb compiled, 1.2mb with libFLAC
- Small memory foorprint: no more than 2.5mb for the flac files
- Low resource consumption: about 14% CPU usage for FLAC on ipod 4g
- All operations in background with GCD
- Event-based (using dispatch sources and CoreAudio async api)
- Provides full audio metadata (vorbis, id3 tags) with embeeded cover images
- HTTP data caching
- Tested (currently about 85% code coverage)

Overview
-------

 Static library and [cocoapods](cocoapods.org) `podspec` provided. Static library can be compiled with embeeded FLAC library (check project targets). For OSX you can use static framework.

 Start playback:

    self.player = [[ORGMEngine alloc] init];
    NSURL* url = [NSURL URLWithString:tfUrl.text];
    [_player playUrl:url];

Common operations:

    [_player metadata];                         // current metadata
    [_player pause];                            // pause playback
    [_player resume];                           // resume playback
    [_player stop];                             // stop playback
    [_player seekToTime:seekSlider.value];      // seek to second
    [_player setNextUrl:url withDataFlush:YES]; // play next track and clear current buffer

Delegate methods:

    - (NSURL*)engineExpectsNextUrl:(ORGMEngine*)engine; // provides continious playback
    - (void)engine:(ORGMEngine*)engine didChangeState:(ORGMEngineState)state; // state change callback

 Check example project and tests for the additional information.

Documentation
-------

Project headers contain `appledoc` comments, precompiled docset [here](http://ap4y.github.com/OrigamiEngine/Documentation/index.html).

Tests
-------

`OCUnit` tests included into the project.

Credits
-------

- http://cogx.org/ by Vincent Spader. FLAC decoder implementation based on `Cog` sources

License
-------
(MIT license)
