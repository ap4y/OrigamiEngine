#import "ORGMCommonProtocols.h"

/**
 `CueSheetDecoder` is a decoder plugin that provides support of the `cue` extension. This decoder provides support for `single-file cue` (multiple tracks within one `flac` file), thus url fragments are used for addressing files within a `cue` sheet. For example, `single_file.cue#3` is a path for the third `flac` track within a `cue` file named `single_file.cue`. Basically, this class is a wrapper for FlacDecoder, it only provides total frame number calculation. If you need to play `multi-file cue` (each track have it's own `flac` file, in this case `cue` is a container) you should parse `cue` file with CueSheetContainer and decode tracks via FlacDecoder.
 */
@interface CueSheetDecoder : NSObject <ORGMDecoder>
@end
