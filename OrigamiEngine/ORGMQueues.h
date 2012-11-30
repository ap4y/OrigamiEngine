//
//  ORGMQueues.h
//  Origami-engine
//
//  Created by ap4y on 8/5/12.
//
//

#import <Foundation/Foundation.h>

/**
 The `ORGMQueues` class contains shared singletons instances of the `GCD` queues and sources. It is not recommended to use this singletons outside the library classes. Description provided for clarification.
 */
@interface ORGMQueues : NSObject

/**
 Returns serial queue used as lock for the concurrent operations.
 
 @discussion Serial queue as a tradition lock provides similar functionality as `@synchronized` but it is significantly faster.
 
 @return Singleton of the `GCD` serial queue.
 */
+ (dispatch_queue_t)lock_queue;

/**
 Returns serial queue used for the engine concurrent operations.
 
 @discussion This queue used by ORGMEngine instances to prevent racing conditions between engine actions.
 
 @return Singleton of the `GCD` serial queue.
 */
+ (dispatch_queue_t)processing_queue;

/**
 Returns `GCD` source used to make playback API event-based rather than utilise locks.

 @discussion Due to the nature of processes, it is clear that decoding is significantly faster than playback (converting + output), thus decoder spends a lot of time waiting for the buffer drainage by output unit. It is easy to check via profiler that waiting intervals are bigger that processing intervals for the decoder in most cases, so event-based operations are more efficient.
 
 @return Singleton of the `GCD` source.
 */
+ (dispatch_source_t)buffering_source;
@end
