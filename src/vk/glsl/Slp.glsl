layout(local_size_x = 64) in;

layout(push_constant) uniform PushConstants {
    float data[]; // Minimal size to hold two vec4s (8 floats), but CPU can push more
} pc;

layout(binding = 0) buffer Result {
    vec4 result[];
};

#ifndef ACT
#define ACT(x)		(x)
#endif

#ifndef ACT_DERIV
#define ACT_DERIV(x)	(x)
#endif

#ifndef LOSS_DERIV
#error LOSS_DERIV is not defined
#endif

void kPredict() {}
void kTrain() {}
void kFollow() {}
void kFit() {}
