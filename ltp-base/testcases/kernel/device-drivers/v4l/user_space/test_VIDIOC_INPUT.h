/*
 * v4l-test: Test environment for Video For Linux Two API
 *
 * 22 Dec 2008  0.2  Test case with NULL parameter added
 * 18 Dec 2008  0.1  First release
 *
 * Written by M�rton N�meth <nm127@freemail.hu>
 * Released under GPL
 */

void test_VIDIOC_G_INPUT(void);
void test_VIDIOC_S_INPUT_from_enum(void);
void test_VIDIOC_S_INPUT_invalid_inputs(void);

void test_VIDIOC_G_INPUT_NULL(void);
void test_VIDIOC_S_INPUT_NULL(void);
