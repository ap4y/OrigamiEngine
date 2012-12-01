//
// CueSheetDecoder.h
//
// Copyright (c) 2012 ap4y (lod@pisem.net)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import "ORGMCommonProtocols.h"

/**
 `CueSheetDecoder` is a decoder plugin that provides support of the `cue` extension. This decoder provides support for `single-file cue` (multiple tracks within one `flac` file), thus url fragments are used for addressing files within a `cue` sheet. For example, `single_file.cue#3` is a path for the third `flac` track within a `cue` file named `single_file.cue`. Basically, this class is a wrapper for FlacDecoder, it only provides total frame number calculation. If you need to play `multi-file cue` (each track have it's own `flac` file, in this case `cue` is a container) you should parse `cue` file with CueSheetContainer and decode tracks via FlacDecoder.
 */
@interface CueSheetDecoder : NSObject <ORGMDecoder>
@end
