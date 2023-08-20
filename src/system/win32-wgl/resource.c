#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "breadbox.h"

// Assuming I did my math right, this should be the valid lookup table for
// CRC32C. If it isn't, somebody please poke me. The code I wrote to generate
// these did NOT work first try. ~Alex
const uint32_t CRC32C_TABLE[256] = {
    0x00000000, 0x0A5F4D75, 0x14BE9AEA, 0x1EE1D79F, // 0x00 - 0x0F
    0x14C5EB57, 0x1E9AA622, 0x007B71BD, 0x0A243CC8,
    0x1433082D, 0x1E6C4558, 0x008D92C7, 0x0AD2DFB2,
    0x00F6E37A, 0x0AA9AE0F, 0x14487990, 0x1E1734E5,
    0x15DECED9, 0x1F8183AC, 0x01605433, 0x0B3F1946, // 0x10 - 0x1F
    0x011B258E, 0x0B4468FB, 0x15A5BF64, 0x1FFAF211,
    0x01EDC6F4, 0x0BB28B81, 0x15535C1E, 0x1F0C116B,
    0x15282DA3, 0x1F7760D6, 0x0196B749, 0x0BC9FA3C,
    0x16054331, 0x1C5A0E44, 0x02BBD9DB, 0x08E494AE, // 0x20 - 0x2F
    0x02C0A866, 0x089FE513, 0x167E328C, 0x1C217FF9,
    0x02364B1C, 0x08690669, 0x1688D1F6, 0x1CD79C83,
    0x16F3A04B, 0x1CACED3E, 0x024D3AA1, 0x081277D4,
    0x03DB8DE8, 0x0984C09D, 0x17651702, 0x1D3A5A77, // 0x30 - 0x3F
    0x171E66BF, 0x1D412BCA, 0x03A0FC55, 0x09FFB120,
    0x17E885C5, 0x1DB7C8B0, 0x03561F2F, 0x0909525A,
    0x032D6E92, 0x097223E7, 0x1793F478, 0x1DCCB90D,
    0x11B258E1, 0x1BED1594, 0x050CC20B, 0x0F538F7E, // 0x40 - 0x4F
    0x0577B3B6, 0x0F28FEC3, 0x11C9295C, 0x1B966429,
    0x058150CC, 0x0FDE1DB9, 0x113FCA26, 0x1B608753,
    0x1144BB9B, 0x1B1BF6EE, 0x05FA2171, 0x0FA56C04,
    0x046C9638, 0x0E33DB4D, 0x10D20CD2, 0x1A8D41A7, // 0x50 - 0x5F
    0x10A97D6F, 0x1AF6301A, 0x0417E785, 0x0E48AAF0,
    0x105F9E15, 0x1A00D360, 0x04E104FF, 0x0EBE498A,
    0x049A7542, 0x0EC53837, 0x1024EFA8, 0x1A7BA2DD,
    0x07B71BD0, 0x0DE856A5, 0x1309813A, 0x1956CC4F, // 0x60 - 0x6F
    0x1372F087, 0x192DBDF2, 0x07CC6A6D, 0x0D932718,
    0x138413FD, 0x19DB5E88, 0x073A8917, 0x0D65C462,
    0x0741F8AA, 0x0D1EB5DF, 0x13FF6240, 0x19A02F35,
    0x1269D509, 0x1836987C, 0x06D74FE3, 0x0C880296, // 0x70 - 0x7F
    0x06AC3E5E, 0x0CF3732B, 0x1212A4B4, 0x184DE9C1,
    0x065ADD24, 0x0C059051, 0x12E447CE, 0x18BB0ABB,
    0x129F3673, 0x18C07B06, 0x0621AC99, 0x0C7EE1EC,
    0x1EDC6F41, 0x14832234, 0x0A62F5AB, 0x003DB8DE, // 0x80 - 0x8F
    0x0A198416, 0x0046C963, 0x1EA71EFC, 0x14F85389,
    0x0AEF676C, 0x00B02A19, 0x1E51FD86, 0x140EB0F3,
    0x1E2A8C3B, 0x1475C14E, 0x0A9416D1, 0x00CB5BA4,
    0x0B02A198, 0x015DECED, 0x1FBC3B72, 0x15E37607, // 0x90 - 0x9F
    0x1FC74ACF, 0x159807BA, 0x0B79D025, 0x01269D50,
    0x1F31A9B5, 0x156EE4C0, 0x0B8F335F, 0x01D07E2A,
    0x0BF442E2, 0x01AB0F97, 0x1F4AD808, 0x1515957D,
    0x08D92C70, 0x02866105, 0x1C67B69A, 0x1638FBEF, // 0xA0 - 0xAF
    0x1C1CC727, 0x16438A52, 0x08A25DCD, 0x02FD10B8,
    0x1CEA245D, 0x16B56928, 0x0854BEB7, 0x020BF3C2,
    0x082FCF0A, 0x0270827F, 0x1C9155E0, 0x16CE1895,
    0x1D07E2A9, 0x1758AFDC, 0x09B97843, 0x03E63536, // 0xB0 - 0xBF
    0x09C209FE, 0x039D448B, 0x1D7C9314, 0x1723DE61,
    0x0934EA84, 0x036BA7F1, 0x1D8A706E, 0x17D53D1B,
    0x1DF101D3, 0x17AE4CA6, 0x094F9B39, 0x0310D64C,
    0x0F6E37A0, 0x05317AD5, 0x1BD0AD4A, 0x118FE03F, // 0xC0 - 0xCF
    0x1BABDCF7, 0x11F49182, 0x0F15461D, 0x054A0B68,
    0x1B5D3F8D, 0x110272F8, 0x0FE3A567, 0x05BCE812,
    0x0F98D4DA, 0x05C799AF, 0x1B264E30, 0x11790345,
    0x1AB0F979, 0x10EFB40C, 0x0E0E6393, 0x04512EE6, // 0xD0 - 0xDF
    0x0E75122E, 0x042A5F5B, 0x1ACB88C4, 0x1094C5B1,
    0x0E83F154, 0x04DCBC21, 0x1A3D6BBE, 0x106226CB,
    0x1A461A03, 0x10195776, 0x0EF880E9, 0x04A7CD9C,
    0x196B7491, 0x133439E4, 0x0DD5EE7B, 0x078AA30E, // 0xE0 - 0xEF
    0x0DAE9FC6, 0x07F1D2B3, 0x1910052C, 0x134F4859,
    0x0D587CBC, 0x070731C9, 0x19E6E656, 0x13B9AB23,
    0x199D97EB, 0x13C2DA9E, 0x0D230D01, 0x077C4074,
    0x0CB5BA48, 0x06EAF73D, 0x180B20A2, 0x12546DD7, // 0xF0 - 0xFF
    0x1870511F, 0x122F1C6A, 0x0CCECBF5, 0x06918680,
    0x1886B265, 0x12D9FF10, 0x0C38288F, 0x066765FA,
    0x0C435932, 0x061C1447, 0x18FDC3D8, 0x12A28EAD
};

int breadbox_resource_load(breadbox_resource_t *res, const char *id) {
    FILE *bbr;
    void *data;
    breadbox_resource_header_t header;
    size_t length;
    // I'm using the filesystem to load resources for now. Just keep in mind
    // that this won't be the case in the final version of the engine! ~Alex
    if(bbr = fopen(id, "r")) {
        fseek(bbr, 0, SEEK_END);
        if((length = ftell(bbr)) > sizeof(breadbox_resource_header_t)) {
            fseek(bbr, 0, SEEK_SET);
            if(fread(&header, sizeof(breadbox_resource_header_t), 1, bbr)) {
                // *Insert whole rant about endianness here*
                // See resources.h for details. ~Alex
                if(header.signature == 0x00524242) {
                    if(header.platform == BBPLAT_NEUTRAL || header.platform == BBPLAT_UNIX_GLX) {
                        if(data = malloc(header.size)) {
                            if(fread(data, 1, header.size, bbr)) {
                                res->data = data;
                                res->hash = header.hash;
                                res->size = header.size;
                                res->type = header.type;
                                if(!breadbox_resource_verify(res)) {
                                    fclose(bbr);
                                    return 0;
                                }
                                res->data = NULL;
                            }
                            free(data);
                        }
                    }
                }
            }
        }
        fclose(bbr);
    }
    return 1;
}

void breadbox_resource_unload(breadbox_resource_t *res) {
    free(res->data);
    res->data = NULL;
}

int breadbox_resource_verify(breadbox_resource_t *res) {
    uint32_t check = 0xFFFFFFFF;
    for(int i = 0; i < res->size; i++) {
        check = CRC32C_TABLE[(char)check ^ ((char *)res->data)[i]] ^ (check >> 8);
    }
    check ^= 0xFFFFFFFF;
    printf("check = %x\n", check);
    return res->hash != check;
}