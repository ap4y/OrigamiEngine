Pod::Spec.new do |s|
  s.name           = "OrigamiEngine"
  s.version        = "1.0.1"
  s.summary        = "Lightweight iOS audio engine with flac, cue, mp3, m4a, m3u support."
  s.homepage       = "https://github.com/ap4y/OrigamiEngine.git"
  s.license        = 'MIT'
  s.author         = { "ap4y" => "lod@pisem.net" }
  s.source         = { :git => "https://github.com/ap4y/OrigamiEngine.git", :tag => "1.0.1" }
  s.platform       = :ios
  s.source_files   = 'OrigamiEngine/**/*.{h,m}'
  s.preserve_paths = 'flac'
  s.xcconfig       = { 'HEADER_SEARCH_PATHS'  => '"$(SRCROOT)/OrigamiEngine/flac/headers"',
                       'LIBRARY_SEARCH_PATHS' => '"$(SRCROOT)/OrigamiEngine/flac"'}
  s.library        = 'FLAC'
end
