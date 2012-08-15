//
//  ORGMQueues.m
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import "ORGMQueues.h"

@implementation ORGMQueues

+ (dispatch_queue_t)lock_queue {
    static dispatch_queue_t _lock_queue;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _lock_queue = dispatch_queue_create("com.origami.lock",
                                            DISPATCH_QUEUE_SERIAL);
    });
    return _lock_queue;
}

+ (dispatch_queue_t)processing_queue {
    static dispatch_queue_t _processing_queue;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _processing_queue = dispatch_queue_create("com.origami.processing",
                                                  DISPATCH_QUEUE_SERIAL);
    });
    return _processing_queue;
}

+ (dispatch_source_t)buffering_source {
    static dispatch_source_t _buffering_source;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _buffering_source = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD,
                                                   0, 0, [ORGMQueues processing_queue]);
        dispatch_resume(_buffering_source);
    });
    return _buffering_source;
}

@end
