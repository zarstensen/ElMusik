
static uint8_t NOTES_TO_PWM0[] =
{
  250, //C3
  249, // C#3
  249, //D3
  248, //D#3
  247, //E3
  246, //F3
  245, //F#3
  244, //G3
  244, // G#3
  242, // A3
  241, // A#3
  240, //B3
  239, //C4
  238, //C#4
  237, //D4
  235, //D#4
  233, //E4
  231, // F4
  230, //F#4
  228, //G4
  226, //G#4
  224, //A4
  221, //AS4
  219, //B4
  217, //C5
  214, // C#5
  211, //D5
  208, //D#5
  205, //E5
  202, //F5
  198, //F#5
  195, //G5
  191, // G#5
  186, // A5
  182, // A#5
  177, //B5
  172, //C6
  167, //C#6
  161, //D6
  156, //D#6
  149, //E6
  143, // F6
  136, //F#6
  129, //G6
  121, //G#6
  113, //A6
  106, //AS6
  99, //B6
  91, //C7  
};

static uint8_t NOTES_TO_PWM1[] =
{
  251, //C3
  250, // C#3
  249, //D3
  249, //D#3
  248, //E3
  247, //F3
  246, //F#3
  245, //G3
  244, // G#3
  243, // A3
  242, // A#3
  241, //B3
  239, //C4
  238, //C#4
  237, //D4
  235, //D#4
  234, //E4
  232, // F4
  231, //F#4
  229, //G4
  227, //G#4
  225, //A4
  223, //AS4
  221, //B4
  218, //C5
  215, // C#5
  213, //D5
  210, //D#5
  207, //E5
  204, //F5
  200, //F#5
  197, //G5
  193, // G#5
  189, // A5
  185, // A#5
  180, //B5
  175, //C6
  171, //C#6
  165, //D6
  160, //D#6
  153, //E6
  147, // F6
  140, //F#6
  134, //G6
  126, //G#6
  118, //A6
  110, //AS6
  103, //B6
  94, //C7  
};

static uint8_t NOTES_TO_PWM2[] =
{
  251, //C3
  250, // C#3
  250, //D3
  249, //D#3
  248, //E3
  247, //F3
  246, //F#3
  245, //G3
  244, // G#3
  243, // A3
  242, // A#3
  241, //B3
  240, //C4
  239, //C#4
  238, //D4
  236, //D#4
  235, //E4
  233, // F4
  231, //F#4
  229, //G4
  227, //G#4
  225, //A4
  223, //AS4
  221, //B4
  219, //C5
  216, // C#5
  213, //D5
  210, //D#5
  207, //E5
  204, //F5
  201, //F#5
  197, //G5
  193, // G#5
  189, // A5
  185, // A#5
  180, //B5
  175, //C6
  170, //C#6
  165, //D6
  159, //D#6
  153, //E6
  147, // F6
  140, //F#6
  134, //G6
  126, //G#6
  118, //A6
  110, //AS6
  103, //B6
  94, //C7  
};

static uint8_t NOTES_TO_PWM3[] =
{
  250, //C3
  249, // C#3
  248, //D3
  248, //D#3
  247, //E3
  246, //F3
  245, //F#3
  244, //G3
  243, // G#3
  242, // A3
  241, // A#3
  239, //B3
  238, //C4
  237, //C#4
  235, //D4
  234, //D#4
  232, //E4
  230, // F4
  229, //F#4
  227, //G4
  225, //G#4
  223, //A4
  220, //AS4
  218, //B4
  216, //C5
  213, // C#5
  210, //D5
  207, //D#5
  204, //E5
  201, //F5
  197, //F#5
  193, //G5
  189, // G#5
  185, // A5
  181, // A#5
  176, //B5
  171, //C6
  166, //C#6
  160, //D6
  155, //D#6
  149, //E6
  142, // F6
  135, //F#6
  128, //G6
  120, //G#6
  113, //A6
  105, //AS6
  99, //B6
  89, //C7  
};

static uint8_t NOTES_TO_PWM4[] =
{
  251, //C3
  250, // C#3
  249, //D3
  249, //D#3
  248, //E3
  247, //F3
  246, //F#3
  245, //G3
  244, // G#3
  243, // A3
  242, // A#3
  241, //B3
  239, //C4
  238, //C#4
  237, //D4
  235, //D#4
  234, //E4
  232, // F4
  230, //F#4
  229, //G4
  227, //G#4
  225, //A4
  223, //AS4
  220, //B4
  218, //C5
  215, // C#5
  213, //D5
  210, //D#5
  207, //E5
  203, //F5
  200, //F#5
  197, //G5
  193, // G#5
  189, // A5
  185, // A#5
  180, //B5
  175, //C6
  171, //C#6
  165, //D6
  160, //D#6
  153, //E6
  147, // F6
  142, //F#6
  135, //G6
  127, //G#6
  120, //A6
  112, //AS6
  106, //B6
  98, //C7  
};

static uint8_t* NOTES_TO_PWMS[] = {NOTES_TO_PWM0, NOTES_TO_PWM1, NOTES_TO_PWM2, NOTES_TO_PWM3, NOTES_TO_PWM4 };

enum class Notes : uint8_t
{
    C3 = 0,
    CS3,
    D3,
    DS3,
    E3,
    F3,
    FS3,
    G3,
    GS3,
    A3,
    AS3,
    B3 ,
    C4,
    CS4,
    D4,
    DS4,
    E4,
    F4,
    FS4,
    G4,
    GS4,
    A4,
    AS4,
    B4,
    C5,
    CS5,
    D5,
    DS5,
    E5,
    F5,
    FS5,
    G5,
    GS5,
    A5,
    AS5,
    B5,
    C6,
    CS6,
    D6,
    DS6,
    E6,
    F6,
    FS6,
    G6,
    GS6,
    A6,
    AS6,
    B6,
    C7,
    OFF,
};