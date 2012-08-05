//
//  ORGMQueues.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import <Foundation/Foundation.h>

@interface ORGMQueues : NSObject
+ (dispatch_queue_t)lock_queue;
+ (dispatch_queue_t)processing_queue;
+ (dispatch_source_t)buffering_source;
@end
