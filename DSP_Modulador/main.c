#include <stdio.h>
#include <string.h>
#include <cycles.h>

#define SCALE 16384

/* ------------------------------------------------------------------
   Função de modulação AM
   x_mod = (mod * carrier / 16384) + carrier
-------------------------------------------------------------------*/
static inline short mod_am(short mod, short carrier)
{
    // 1) Converte para 32 bits para evitar overflow
    long m = (long)mod;
    long c = (long)carrier;

    // 2) Multiplicação moduladora * portadora
    long prod = (m * c) / SCALE;

    // 3) Soma com a portadora
    long out = prod + c;

    // 4) Clipping para int16
    if (out > 32767)  out = 32767;
    else if (out < -32768) out = -32768;

    return (short)out;
}

/* ------------------------------------------------------------------
   MAIN — Processamento AM
-------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    cycle_stats_t stats;
    FILE *fmod, *fcar, *fout;
    short mod_in, car_in, out;

    printf("***************************************************************\n");
    printf("* MODULACAO AM PARA DSP BLACKFIN                              *\n");
    printf("***************************************************************\n\n");

    // Arquivo moduladora (onda baixa frequência)
    fmod = fopen("..\\input_modulador.pcm", "rb");
    if (!fmod)
    {
        printf("Erro ao abrir input_modulador.pcm\n");
        return 0;
    }

    // Arquivo portadora (onda de alta frequência)
    fcar = fopen("..\\input_portador.pcm", "rb");
    if (!fcar)
    {
        printf("Erro ao abrir input_portador.pcm\n");
        fclose(fmod);
        return 0;
    }

    fout = fopen("..\\output_modulado.pcm", "wb");
    if (!fout)
    {
        printf("Erro ao abrir output_modulado.pcm\n");
        fclose(fmod);
        fclose(fcar);
        return 0;
    }

    CYCLES_INIT(stats);

    printf("Processando...\n");

    while (1)
    {
        // Lê uma amostra da moduladora e uma da portadora
        if (fread(&mod_in, sizeof(short), 1, fmod) != 1)
            break;
        if (fread(&car_in, sizeof(short), 1, fcar) != 1)
            break;

        CYCLES_START(stats);

        out = mod_am(mod_in, car_in);

        CYCLES_STOP(stats);

        fwrite(&out, sizeof(short), 1, fout);
    }

    printf("Terminado.\n");

    CYCLES_PRINT(stats);

    fclose(fmod);
    fclose(fcar);
    fclose(fout);

    return 0;
}
