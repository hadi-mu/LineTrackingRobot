void camera_true(void);
void camera_init(void);
void camera_dummy(void);
void camera_read(void);
int camera_max(unsigned char CCD[]);
void camera_process(unsigned char *l, unsigned char *r);
int checkLost(unsigned char *l, unsigned char *r);
int checkTurn(unsigned int check);
int checkObst(void);
