#include <stdio.h>
#include <string.h>
#include <cycles.h>

extern float demod_am_float(float entrada);

int main(int argc, char *argv[])
{
    cycle_stats_t stats;
    FILE *fin, *fout;

    short entrada_short;
    float entrada_f, saida_f;
    short saida_short;

    printf("***************************************************************\n");
    printf("* DEMODULACAO AM (VERSAO FLOAT) PARA DSP BLACKFIN             *\n");
    printf("***************************************************************\n\n");

    fin = fopen("..\\am_input.pcm", "rb");
    if (!fin)
    {
        printf("Erro ao abrir entrada.\n");
        return 0;
    }

    fout = fopen("..\\am_output_float.pcm", "wb");
    if (!fout)
    {
        printf("Erro ao abrir saida.\n");
        return 0;
    }

    CYCLES_INIT(stats);

    printf("Processando em FLOAT...\n");

    while (fread(&entrada_short, sizeof(short), 1, fin) == 1)
    {
        // Converte short para float na faixa [-1.0, +1.0]
        entrada_f = (float)entrada_short / 32768.0f;

        CYCLES_START(stats);

        // Processamento em ponto flutuante
        saida_f = demod_am_float(entrada_f);

        CYCLES_STOP(stats);

        // Converte float de volta para short
        if (saida_f > 1.0f)  saida_f = 1.0f;
        if (saida_f < -1.0f) saida_f = -1.0f;

        saida_short = (short)(saida_f * 32767.0f);

        fwrite(&saida_short, sizeof(short), 1, fout);
    }

    printf("Terminado.\n");

    CYCLES_PRINT(stats);

    fclose(fin);
    fclose(fout);

    return 0;
}
