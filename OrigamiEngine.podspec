Pod::Spec.new do |s|
  s.name           = "OrigamiEngine"
  s.version        = "1.0.5"
  s.summary        = "Lightweight iOS/OSX audio engine with flac, cue, mp3, m4a, m3u support."
  s.homepage       = "https://github.com/ap4y/OrigamiEngine.git"
  s.license        = 'MIT'
  s.author         = { "ap4y" => "lod@pisem.net" }
  s.source         = { :git => "https://github.com/ap4y/OrigamiEngine.git", :tag => "1.0.5", :submodules => true }
  s.source_files   = 'OrigamiEngine/**/*.{h,m}'
  s.preserve_paths = 'Audio-Frameworks/bin/flac/'

  s.ios.deployment_target = '5.0'
  s.ios.frameworks        = 'AudioToolbox', 'AVFoundation', 'FLAC'
  s.ios.xcconfig          = { 'FRAMEWORK_SEARCH_PATHS' => '"$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/flac/" "$(SDKROOT)/Developer/Library/Frameworks" "$(DEVELOPER_LIBRARY_DIR)/Frameworks"'}

  s.osx.deployment_target = '10.7'
  s.osx.frameworks        = 'AudioToolbox', 'AVFoundation', 'AudioUnit', 'FLAC'
  s.osx.xcconfig          = { 'FRAMEWORK_SEARCH_PATHS' => '"$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/flac/FLAC_OSX/" "$(SDKROOT)/Developer/Library/Frameworks" "$(DEVELOPER_LIBRARY_DIR)/Frameworks"'}
end
