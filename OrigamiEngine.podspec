Pod::Spec.new do |s|
  s.name           = "OrigamiEngine"
  s.version        = "1.0.9"
  s.summary        = "Lightweight iOS/OSX audio engine with flac, cue, mp3, m4a, m3u support."
  s.homepage       = "https://github.com/ap4y/OrigamiEngine.git"
  s.license        = 'MIT'
  s.author         = { "ap4y" => "lod@pisem.net" }
  s.source         = { :git => "https://github.com/ap4y/OrigamiEngine.git", :tag => "1.0.9", :submodules => true }
  s.source_files   = 'OrigamiEngine/**/*.{h,m}'

  s.ios.deployment_target = '5.0'
  s.ios.preserve_paths    = 'Audio-Frameworks/bin/flac/FLAC.framework'
  s.ios.frameworks        = 'AudioToolbox', 'AVFoundation', 'FLAC', 'Ogg', 'Opus', 'OpusFile'
  s.ios.xcconfig          = { 'FRAMEWORK_SEARCH_PATHS' => '"$(SDKROOT)/Developer/Library/Frameworks" "$(DEVELOPER_LIBRARY_DIR)/Frameworks" "$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/flac/"' }

  s.osx.deployment_target = '10.7'
  s.osx.preserve_paths    = 'Audio-Frameworks/bin/flac/Flac_OSX/FLAC.framework'
  s.osx.frameworks        = 'AudioToolbox', 'AVFoundation', 'AudioUnit', 'FLAC'
  s.osx.xcconfig          = { 'FRAMEWORK_SEARCH_PATHS' => '"$(SDKROOT)/Developer/Library/Frameworks" "$(DEVELOPER_LIBRARY_DIR)/Frameworks" "$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/flac/FLAC_OSX"' }

  s.subspec 'Opus' do |opus|
      opus.frameworks            = 'Ogg', 'Opus', 'OpusFile'

      opus.ios.preserve_paths    = [
          'Audio-Frameworks/bin/ogg/Ogg.framework',
          'Audio-Frameworks/bin/opus/Opus.framework',
          'Audio-Frameworks/bin/opusfile/OpusFile.framework',
          'Audio-Frameworks/bin/opus/include',
      ]
      opus.ios.xcconfig          = { 
          'FRAMEWORK_SEARCH_PATHS' => [
              '"$(SDKROOT)/Developer/Library/Frameworks"',
              '"$(DEVELOPER_LIBRARY_DIR)/Frameworks"',
              '"$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/ogg/"',
              '"$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/opus/"',
              '"$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/opusfile/"'
          ].join(' '),
          'HEADER_SEARCH_PATHS' => '"$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/opus/include"'
      }

      opus.osx.preserve_paths    = [
          'Audio-Frameworks/bin/ogg/Ogg.framework',
          'Audio-Frameworks/bin/ogg/MacOS/Ogg.framework',
          'Audio-Frameworks/bin/opus/MacOS/Opus.framework',
          'Audio-Frameworks/bin/opusfile/MacOS/OpusFile.framework'
      ]
      opus.osx.xcconfig          = { 
          'FRAMEWORK_SEARCH_PATHS' => [
              '"$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/ogg/MacOS"',
              '"$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/opus/MacOS"',
              '"$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/opusfile/MacOS"'
          ].join(' ')
      }
  end

end
