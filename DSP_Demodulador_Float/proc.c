#include <math.h>

// Parâmetros equivalentes, mas adaptados para FLOAT
#define DECAY_BASE_FLOAT   7.0f   // para manter a lógica do <<7
#define DECAY_SCALE        128.0f // 2^7
#define K_UP_DIV           32.0f  // 1 << 5   (k_up = 5)
#define K_DOWN_DIV          2.0f  // 1 << 1   (k_down = 1)

// Estados internos
static float envelope_state_f = 0.0f;
static float envelope_out_f   = 0.0f;

float demod_am_float(float entrada)
{
    float x = fabsf(entrada);

    // Equivalente ao shift (<< DECAY_BASE)
    x *= DECAY_SCALE;

    // Envelope BRUTO
    if (x > envelope_state_f)
        envelope_state_f = x;
    else
        envelope_state_f -= (envelope_state_f / DECAY_SCALE);

    // Converte de volta (>> DECAY_BASE)
    float env_short_f = envelope_state_f / DECAY_SCALE;

    // Suavização dinâmica
    if (env_short_f > envelope_out_f)
        envelope_out_f += (env_short_f - envelope_out_f) / K_UP_DIV;
    else
        envelope_out_f -= (envelope_out_f - env_short_f) / K_DOWN_DIV;

    // Normalização final em float (-1 a +1)
    float out_f = envelope_out_f * 2.0f - 1.0f;

    return out_f;
}
