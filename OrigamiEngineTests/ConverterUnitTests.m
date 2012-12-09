//
// ConverterUnitTests.m
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

#import "ConverterUnitTests.h"

#import "ORGMConverter.h"
#import "ORGMInputUnit.h"
#import "ORGMOutputUnit.h"

@interface ConverterUnitTests ()
@property (retain, nonatomic) ORGMConverter *converter;
@end

@implementation ConverterUnitTests

- (void)setUp {
    [super setUp];
    ORGMInputUnit *input = [[ORGMInputUnit alloc] init];
    NSURL *flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                            withExtension:@"flac"];
    [input openWithUrl:flacUrl];
    _converter = [[ORGMConverter alloc] initWithInputUnit:input];
    
    ORGMOutputUnit *output = [[ORGMOutputUnit alloc] initWithConverter:_converter];
    STAssertTrue([_converter setupWithOutputUnit:output], nil);        
    
    [input release];
    [output release];
}

- (void)tearDown {
    [_converter release];
    [super tearDown];
}

- (void)testConverterUnitShouldHaveValidInputUnit {
    STAssertNotNil(_converter.inputUnit, nil);
}

- (void)testConverterUnitShouldHaveValidOutputUnit {
    STAssertNotNil(_converter.outputUnit, nil);
}

- (void)testConverterUnitShouldProcessData {
    [_converter.inputUnit process];
    [_converter process];
    STAssertEquals(_converter.convertedData.length, 131072U, nil);
}

- (void)testInputUnitShouldNotExceedMaxAmountInBuffer {
    [_converter.inputUnit process];
    [_converter process];
    NSUInteger _saveLength = _converter.convertedData.length;
    [_converter.inputUnit process];
    [_converter process];
    STAssertEquals(_converter.convertedData.length, _saveLength, nil);
}

- (void)testConverterUnitshouldReinitWithNewInputUnit {
    [_converter.inputUnit process];
    [_converter process];
    NSUInteger _saveLength = _converter.convertedData.length;
    
    ORGMInputUnit *input = [[ORGMInputUnit alloc] init];
    NSURL *flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                            withExtension:@"flac"];
    [input openWithUrl:flacUrl];
    [_converter reinitWithNewInput:input withDataFlush:NO];
    
    STAssertEquals(_converter.inputUnit, input, nil);
    STAssertEquals(_converter.convertedData.length, _saveLength, nil);
    [input release];
}

- (void)testConverterUnitshouldReinitWithNewInputUnitAndFlushData {
    [_converter.inputUnit process];
    [_converter process];
    
    ORGMInputUnit *input = [[ORGMInputUnit alloc] init];
    NSURL *flacUrl = [[NSBundle bundleForClass:self.class] URLForResource:@"multiple-vc"
                                                            withExtension:@"flac"];
    [input openWithUrl:flacUrl];
    [_converter reinitWithNewInput:input withDataFlush:YES];
    
    STAssertEquals(_converter.inputUnit, input, nil);
    STAssertEquals(_converter.convertedData.length, 0U, nil);
    [input release];
}

@end
