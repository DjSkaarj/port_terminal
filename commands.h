#ifndef COMMANDS_H_
#define COMMANDS_H_

// message header definitions
#define ACK 'A' // Acknowledgement
#define UART 'U'
#define GPIO 'G'
#define INPERRUPT 'I'
#define ERROR 'E' 
#define BEGIN 'b'
#define END_MESSAGE '\n'
#define TASK 'T'

//#define SETTINGS 'S'
#define SET 'S'
#define GET 'g'

#define VALUE 'V'
#define MODE 'M'

#define HIGH_STATE '\xf0'
#define LOW_STATE '\x0f'

#define OUTPUT_MODE 'O'
#define INPUT_MODE 'i'

#define LOG_INFO 'L'

#endif /* COMMANDS_H_ */
