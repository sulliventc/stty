//
// Created by colten on 05/07/17.
//

#ifndef STTYL_MAIN_H
#define STTYL_MAIN_H

#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>

struct flaginfo {int fl_value; char *fl_name;};

struct flaginfo iflags[] = {
        //{IGNBRK     ,   "ignbrk"},
        //{BRKINT     ,   "brkint"},
        //{IGNPAR     ,   "ignpar"},
        //{PARMRK     ,   "parmrk"},
        //{INPCK      ,   "inpck"},
        //{ISTRIP     ,   "istrip"},
        //{INLCR      ,   "inlcr"},
        //{IGNCR      ,   "igncr"},
        //{IUCLC      ,   "iuclc"},
        //{IXON       ,   "ixon"},
        //{IXANY      ,   "ixany"},
        //{IXOFF      ,   "ixoff"},
        //{IMAXBEL    ,   "imaxbel"},
        {ICRNL      ,   "icrnl"},
        {0          ,   NULL}
};

struct flaginfo oflags[] = {
        //{OPOST      ,   "opost"},
        //{OCRNL      ,   "ocrnl"},
        //{ONLRET     ,   "onlret"},
        //{OFILL      ,   "ofill"},
        //{OFDEL      ,   "ofdel"},
        {ONLCR      ,   "onlcr"},
        {OLCUC      ,   "olcuc"},
        {0          ,   NULL}
};

struct flaginfo lflags[] = {
        //{ECHOK      ,   "echok"},
        //{ECHOKE     ,   "echoke"},
        //{ECHOCTL    ,   "echoctl"},
        //{ECHONL     ,   "echonl"},
        //{NOFLSH     ,   "noflsh"},
        //{TOSTOP     ,   "tostop"},
        //{FLUSHO     ,   "flusho"},
        {ISIG       ,   "isig"},
        {ICANON     ,   "icanon"},
        {ECHOE      ,   "echoe"},
        {ECHO       ,   "echo"},
        {0          ,   NULL}
};

struct flaginfo cflags[] = {
        {CRTSCTS    ,   "crtscts"},
        {0          ,   NULL}
};

struct flaginfo c_cc[] = {
        {VINTR      ,   "intr"},
        {VQUIT      ,   "quit"},
        {VERASE     ,   "erase"},
        {VKILL      ,   "kill"},
        {VMIN       ,   "min"},
        {VTIME      ,   "time"},
        {0          ,   NULL}
};

struct termios;

void show_baud(int);
void show_keys(struct termios *);
void show_flags(struct termios *);
void show_flagset(int, struct flaginfo[]);
int normalize_key(int);
void set_flag(int, int, int, struct termios *);


#endif //STTYL_MAIN_H
