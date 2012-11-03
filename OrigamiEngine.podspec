Pod::Spec.new do |s|
  s.name           = "OrigamiEngine"
  s.version        = "1.0.4"
  s.summary        = "Lightweight iOS audio engine with flac, cue, mp3, m4a, m3u support."
  s.homepage       = "https://github.com/ap4y/OrigamiEngine.git"
  s.license        = 'MIT'
  s.author         = { "ap4y" => "lod@pisem.net" }
  s.source         = { :git => "https://github.com/ap4y/OrigamiEngine.git", :tag => "1.0.4", :submodules => true }
  s.platform       = :ios
  s.source_files   = 'OrigamiEngine/**/*.{h,m}'
  s.preserve_paths = 'Audio-Frameworks/bin/flac/'
  s.xcconfig       = { 'FRAMEWORK_SEARCH_PATHS' => '"$(PODS_ROOT)/OrigamiEngine/Audio-Frameworks/bin/flac/" "$(SDKROOT)/Developer/Library/Frameworks" "$(DEVELOPER_LIBRARY_DIR)/Frameworks"'}
  s.frameworks     = 'AudioToolbox', 'AVFoundation', 'FLAC'
end
