/** @file time.h
 *
 * @brief Declares the global errno variable
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:37:12 -0400
 */

#ifndef TIME_H
#define TIME_H

#ifndef ASSEMBLER

extern volatile unsigned long timer_count;

#endif /* ASSEMBLER */

#endif /* TIME_H */
