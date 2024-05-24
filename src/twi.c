// Eingebettete Systeme  / Embedded Systems
// Embedded Distance Assistent (EDA)
// SEMESTER: SS24
// AUTHORS: David Heiss, Manfred Pester, Jens-Peter Akelbein
// FILE: state.c
// CONTENTS: twi functions

#include <twi.h>
#include <math.h>

void twi_init(Twi *const twi, const uint32_t freq, const bool slave)
{
    if (twi == TWI0)
    {
        PMC->PMC_PCER0 = 1u << ID_TWI0;
        PIOB->PIO_PDR = PIO_PA18A_TWCK0 | PIO_PA17A_TWD0;
        PIOB->PIO_ABSR &= ~(PIO_PA18A_TWCK0 | PIO_PA17A_TWD0);
    }
    else if (twi == TWI1)
    {
        PMC->PMC_PCER0 = 1u << ID_TWI1;
        PIOB->PIO_PDR = PIO_PB13A_TWCK1 | PIO_PB12A_TWD1;
        PIOB->PIO_ABSR &= ~(PIO_PB13A_TWCK1 | PIO_PB12A_TWD1);
    }

    twi->TWI_CR = TWI_CR_SWRST;
    twi->TWI_PTCR = TWI_PTCR_RXTDIS | TWI_PTCR_TXTDIS;

    const uint32_t ckdiv = ceil(log2(((float)SystemCoreClock / freq / 2 - 4) / 255.));
    const uint32_t div = ((float)SystemCoreClock / freq / 2 - 4) / pow(2, ckdiv);
    twi->TWI_CWGR = TWI_CWGR_CKDIV(ckdiv) | TWI_CWGR_CHDIV(div) | TWI_CWGR_CLDIV(div);

    if (slave)
    {
        twi->TWI_CR = TWI_CR_MSDIS | TWI_CR_SVEN;
    }
    else
    {
        twi->TWI_CR = TWI_CR_SVDIS | TWI_CR_MSEN;
    }

    twi->TWI_PTCR = TWI_PTCR_RXTEN | TWI_PTCR_TXTEN;
}

void twi_write(Twi *const twi, const uint32_t address, const uint8_t *const ptr, const uint32_t size)
{
    /*Implementieren Sie die eigentliche Übertragung über I2C inheralb der twi_write Funktion. Verwenden 
     *Sie für die Übertragung das MMR, SR und THR. Alternativ können Sie für die Übertragung auch den 
     *DMA-Controller mit den Registern TPR und TCR verwenden. Die Adresse der LED-Matrix ist 0x70 bis 0x77.
    */

    // 'Master write direction' setzen
   twi->TWI_MMR = 0u << 12 | address << 16;

    // Warten, bis das TWI bereit für eine neue Übertragung ist
    while (!(twi->TWI_SR & TWI_SR_TXRDY));

    // Senden der Start-Bedingung
    twi->TWI_CR = TWI_CR_START;

    // Warten, bis die Start-Bedingung übertragen wurde
    while (!(twi->TWI_SR & TWI_SR_TXRDY));

    // Senden der Daten
    for (uint32_t i = 0; i < size; ++i)
    {
        // Senden des nächsten Datenbytes
        twi->TWI_THR = ptr[i];

        // Warten, bis das Byte übertragen wurde
        while (!(twi->TWI_SR & TWI_SR_TXRDY));
    }

    // Senden der Stop-Bedingung
    twi->TWI_CR = TWI_CR_STOP;

    // Warten, bis die Stop-Bedingung übertragen wurde
    while (!(twi->TWI_SR & TWI_SR_TXCOMP));


}
