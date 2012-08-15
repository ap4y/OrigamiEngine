Pod::Spec.new do |s|
  s.name           = "OrigamiEngine"
  s.version        = "1.0.0"
  s.summary        = "Lightweight iOS audio engine with flac, cue, mp3, m4a, m3u support"
  s.homepage       = "https://github.com/ap4y/OrigamiEngine.git"
  s.license        = 'MIT'
  s.author         = { "ap4y" => "lod@pisem.net" }
  s.source         = { :git => "git://github.com/ap4y/OrigamiEngine.git", :tag => "1.0.0" }
  s.platform       = :ios
  s.source_files   = 'OrigamiEngine'
  s.xcconfig       = { 'HEADER_SEARCH_PATHS'  => '"$(SRCROOT)/ThirdParty/bin/flac/headers"',
                       'LIBRARY_SEARCH_PATHS' => '"$(SRCROOT)/ThirdParty/bin/flac/headers"'}
  s.compiler_flags = '-lFLAC'
end
