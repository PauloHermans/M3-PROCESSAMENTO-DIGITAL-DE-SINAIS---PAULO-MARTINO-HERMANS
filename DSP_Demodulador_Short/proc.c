#include <stdio.h>

#define DECAY_BASE 7        // decay fixo para manter estabilidade
#define AM_ZERO_OFFSET 16384

// Envelope bruto (alta resolução)
static int envelope_state = 0;

// Envelope final suavizado (já em short)
static int envelope_out = 0;

// Constantes variáveis (controlam a “forma”)
static int k_up   = 5;  // mais lento (suave)
static int k_down = 1;   // mais rápido para cair

short demod_am(short entrada)
{
    // -----------------------------------------
    // 1) Retificação absoluta
    // -----------------------------------------
    int x = entrada;
    if (x < 0) 
    {
    	x = -x;
	}
    
    // Expande para resolução maior
    x <<= DECAY_BASE;

    // -----------------------------------------
    // 2) ENVELOPE BRUTO (fixo, sem mudanças bruscas)
    // -----------------------------------------
    if (x > envelope_state)
    {
        envelope_state = x;
    }
    else
    {
        envelope_state -= (envelope_state >> DECAY_BASE);
    }
    
    // -----------------------------------------
    // 3) CONVERTE PARA SHORT (valor de pico real)
    // -----------------------------------------
    int env_short = (envelope_state >> DECAY_BASE);

    // -----------------------------------------
    // 4) SUAVIZAÇÃO DINÂMICA
    // -----------------------------------------
    // Se está subindo:
    if (env_short > envelope_out)
    {
        envelope_out += (env_short - envelope_out) >> k_up;
    }
    else
    {
        // Se está descendo
        envelope_out -= (envelope_out - env_short) >> k_down;
    }

    // -----------------------------------------
    // 5) Normaliza para -32768..32767
    // -----------------------------------------
    short out = (short)((envelope_out - AM_ZERO_OFFSET) << 1);

    return out;
}
