#include "netclientthread.h"

#define             RING_BUFFER_SIZE    19200ul

NetClientThread::NetClientThread( QString server_ip, int server_port )
{
    socket = new QTcpSocket();
    server = new QTcpServer();
    data_packet = (struct data_packet_t*)malloc(sizeof(struct data_packet_t));
    file_ctr = new FileManager();
    array_length = 0;
    mutex = new QMutex();
    count = 0;
    is_head = false;
    packet_number = 0;
    kcount = 0;
    isEnableSave = false;
    is_enable_socket_read = false;
    left_length = 0;
    array_rom.clear();

    connect( this, SIGNAL(net_data_save_to_disk(quint8*,quint64) ),(QObject*)this->file_ctr ,SLOT(on_save_data_to_disk(quint8*,quint64)));
    connect( (QObject*)this->file_ctr, SIGNAL(file_manager_add_file_name(QString)), this, SLOT(on_file_manager_add_doc_list(QString)) );
    connect( (QObject*)this->file_ctr, SIGNAL(file_manager_file_size(double)), this, SLOT(on_file_manager_file_size(double)));
}

bool NetClientThread::set_connect(QString server_ip, int server_port)
{
    //1qDebug() << "netclientread@set_connect() >: seting the connection...." ;
    socket->connectToHost( server_ip, server_port , QIODevice::ReadWrite );

    QObject::connect((QObject*) socket,SIGNAL(readyRead()),(QObject*)this,SLOT(on_read_message()));
    if( !socket->waitForConnected(30000) ) {
        //1qDebug("netclientread@set_connect() >: socket Connection failed!!");
        return false;
    }else {
        //1qDebug("netclientread@set_connect() >: socket conncetion succussful.");
        return true;
    }
}
void NetClientThread::set_disconnet()
{
    socket->flush();
    socket->close();
}

float NetClientThread::bcd_decoding(uint8_t *dHandle)
{
    float *df;
    uint32_t temp;

    for ( int i = 0; i < 4; i ++ ) {
        temp |= *(dHandle + 4 - i) & 0xFF;
        temp = temp << 8;
    }
    df = (float *)&temp;
    return *df;
}

void NetClientThread::bcd_encoding(float df, uint8_t *distChar)
{
    uint8_t* dHandle;

    dHandle = (uint8_t *)&df;
    *distChar = *dHandle & 0xFF;
    *(distChar + 1) = *(dHandle + 1) & 0xFF ;
    *(distChar + 2) = *(dHandle + 2) & 0xFF ;
    *(distChar + 3) = *(dHandle + 3) & 0xFF ;
}

void NetClientThread::send_cmd_close_app()
{
    QByteArray cmd;
    cmd.clear();
    cmd.append(0xAA);
    cmd.append(0xBB);
    cmd.append(0x01);
    cmd.append(0x01);
    cmd.append(0xFF);
    cmd.append(0xAA^0xBB^0x01^0x01^0xFF);
    socket->write(cmd);
    socket->flush();
    //1qDebug() << "netclientread@send_cmd_close_app() >: close remote...." ;
}

void NetClientThread::send_cmd_to_remote(uint8_t *users, quint16 length)
{
    QByteArray cmd;
    quint8  checkSum = 0;
    union {
        struct {
            uint8_t bit0_8;
            uint8_t bit8_16;
        } bits;
        uint16_t all;
    } lengthCmd;

    cmd.clear();
    cmd.append(CMD_HEADER_1);
    cmd.append(CMD_HEADER_2);
    /* cmd length not contains cmd headers(0xAA 0xBB) and length byte. */
    lengthCmd.all = length - 1;
    cmd.append( lengthCmd.bits.bit8_16);
    cmd.append( lengthCmd.bits.bit0_8);
    for (int i = 0; i < length; i ++)
        cmd.append(users[i]);
    //1qDebug() << "send :" << cmd;
    socket->write(cmd);
    socket->flush();
    //1qDebug() << "netclientread@send_cmd_close_app() >: send to remote...." ;
}
// socket ascii stream.
// [0xAD 0xAC] : 2bytes frame heads.
// [0xLL 0xLL 0xLL 0xLL] : 4bytes data number.
// 500 x [0xQQ 0xQQ 0xQQ 0xTT | 0xQQ 0xQQ 0xQQ 0xTT | 0xQQ 0xQQ 0xQQ 0xTT | 0xQQ 0xQQ 0xQQ 0xTT] : 500*16 adc datas
// [0xEE 0xEE 0xEE 0xEE] : 4bytes end flags.
void NetClientThread::run()
{

}
void NetClientThread::on_read_message()
{
    if (is_enable_socket_read == false)
        return;
    qint8 ret = 7;

    array_rom.append(socket->readAll());
    array_length = array_rom.length();
    //qDebug() << "on read message" << array_length;
    if (array_length < 5*ONE_PACKET_LENGTH)
        return;
    if (array_length > 10*8010) {
        qDebug() << "data over size" << array_length;
        //array_rom.clear();
        vector_counter = 0;
    }
    socket_buffer = (quint8*)malloc(sizeof(quint8*) * (array_length + 1) );
    for (quint32 i = 0; i < array_length; i ++) {
        socket_buffer[i] = array_rom.at(i)&0xFF;
    }
    array_rom.clear();
    ret = check_packet(socket_buffer, array_length);
    //qDebug() << "Checked packet : length = " << vector_counter  ;
    if ( ret == -1 ) {
        qDebug() << "Checked failed..........................................................";
    }
    switch(ret) {

    case 1:
        //qDebug() << "case 1: ";
        case_1(socket_buffer, array_length);
        break;

    case 2:
        //qDebug() << "case 2: ";
        case_2(socket_buffer, array_length, left_rom, &left_length);
        break;

    case 3:
        //qDebug() << "case 3: ";
        case_3(socket_buffer, array_length, left_rom, left_length);
        break;

    case 4:
        //qDebug() << "case 4: ";
        case_4(socket_buffer, array_length);
        break;
    }
    free(socket_buffer);
    vector_counter = 0;

    //socket->flush();

}

qint8 NetClientThread::check_packet(quint8* array, quint64 length)
{
    quint16 header_vector_table[9999];
    quint32 vec_counter = 0;


    memset(header_vector_table,0xff,9999);

    header_vector_table[0] = 0xF;
    for (quint32 i = 0; i < length - 1; i ++) {
        if ( ((array[i] & 0xFF)  == 0xad) && (((array[i+1] & 0xFF) == 0xac)) ) {
            header_vector_table[vec_counter] = i;
            vec_counter ++;

        }
    }
    if (vec_counter > 9999) {
        qDebug() << "@check_packet: over the length "<< vec_counter;
    }
    // case 1;
    if ((header_vector_table[0] == 0) && (length%ONE_PACKET_LENGTH == 0)) {
        is_head = true;
        return 1;
    }
    // case 2;
    if ((header_vector_table[0] == 0) && (length%ONE_PACKET_LENGTH != 0)) {
        is_head = true;
        return 2;
    }
    // case 3;
    if ((header_vector_table[0] != 0) && (is_head == true)) {
        return 3;
    }
    // case 4;
    if ((header_vector_table[0] != 0) && (is_head == false)) {
        is_head = true;
        return 4;
    }

    return -1;
}
void NetClientThread::case_1(quint8 *buffer,  quint64 length, quint32 vector_counter)
{
    case_1(buffer, length);
}

void NetClientThread::case_1(quint8 *buffer,  quint64 length)
{

    //qDebug() << "do case 1";
    qint64 count = 0;
    qint32 length_d = (qint32)length;
    qint32 pac_num = 8010;
    quint32 da,db,dc,dd,d_len;
    count = (length_d / 8010);
    ////1qDebug() << "emit net data save to disk singal.";
    if (isEnableSave == true) {
        emit net_data_save_to_disk(buffer, length);
    }

    kcount ++;
    //1qDebug() << "@case_1 : count is :" << count;
    // plot data
    if (kcount % 12 == 0) {
        kcount = 0;
        memset(channel_data,0,2000);
        quint32 uh = 0, h = 0, l = 0, ul = 0;

        for (quint64 i = 0 ; i < 500*16; i ++) {
            plot_buffer[i] = buffer[i+6];
        }

        uh = (plot_buffer[2] << 24) & 0xFF000000;
        h  = (plot_buffer[3] << 16) & 0x00FF0000;
        l  = (plot_buffer[4] << 8)  & 0x0000FF00;
        ul = (plot_buffer[5] << 0)  & 0X000000FF;
        d_len = uh | h | l | ul;

        for (quint64 i = 0; i < 500; i ++) {

            uh = (plot_buffer[16*i + 2] << 16) & 0x00FF0000;
            h  = (plot_buffer[16*i + 1] << 8)  & 0x0000FF00;
            l  = (plot_buffer[16*i + 0] << 0)  & 0x000000FF;
            ul = (plot_buffer[16*i + 3] << 0)  & 0X000000FF;

            da = uh | h | l;

            uh = (plot_buffer[16*i + 6] << 16) & 0x00FF0000;
            h  = (plot_buffer[16*i + 5] << 8)  & 0x0000FF00;
            l  = (plot_buffer[16*i + 4] << 0)  & 0x000000FF;
            ul = (plot_buffer[16*i + 7] << 0)  & 0X000000FF;

            db = uh | h | l;

            uh = (plot_buffer[16*i + 10] << 16) & 0x00FF0000;
            h  = (plot_buffer[16*i + 9] << 8)  & 0x0000FF00;
            l  = (plot_buffer[16*i + 8] << 0)  & 0x000000FF;
            ul = (plot_buffer[16*i + 11] << 0)  & 0X000000FF;

            dc = uh | h | l;

            uh = (plot_buffer[16*i + 14] << 16) & 0x00FF0000;
            h  = (plot_buffer[16*i + 13] << 8)  & 0x0000FF00;
            l  = (plot_buffer[16*i + 12] << 0)  & 0x000000FF;
            ul = (plot_buffer[16*i + 15] << 0)  & 0X000000FF;

            dd = uh | h | l;

            channel_data[i] = da;
            channel_data[500 + i] = db;
            channel_data[1000 + i] = dc;
            channel_data[1500 + i] = dd;
        }
        ////1qDebug() << "emit net data plot signal.";
       emit net_data_plot(channel_data, 2000);
    }


}
void NetClientThread::case_2(quint8 *socket_buffer, quint64 length, quint8 *left_buffer, quint64 *left_length)
{
    //qDebug() << "do case 2";
    quint32 final_head_index = 0;
    // step 1: save left buffer data.

    for (quint64 i = length; i > 0; i --) {
        if ( socket_buffer[i] == 0xAC && socket_buffer[i-1] == 0xAD ) {
            final_head_index = i - 1;
            break;
        }
    }

    *left_length = length - final_head_index + 1;
    // positive direction save.
    for (quint64 i = 0; i < *left_length; i ++) {
        *(left_buffer + i) = *(socket_buffer + final_head_index + i);
    }

    // step 2: save complete data packet.
    //qDebug() << "join case 1";
    case_1(left_buffer, final_head_index);
}

void NetClientThread::case_3(quint8 *socket_buffer, quint64 length, quint8 *right_buffer, quint64 right_length)
{
   // qDebug() << "do case 3";
    quint64 first_header_index = 0;
    quint32 header_vector_table[100];
    quint64 vector_count = 0;
    quint8 *unknown_buffer = new quint8[ONE_PACKET_LENGTH*100];
    quint32 unknown_length = 0;
    memset(header_vector_table,0,100);
    //1qDebug() << "case prosss";
    for (quint64 i = 0; i < length; i ++) {
        if ( (*(socket_buffer + i) == 0xAD) && (*(socket_buffer + i + 1) == 0xAC) ) {
            first_header_index = i;
            header_vector_table[vector_count++] = i;
        }
    }
    qDebug() << "@case3:vector:" << vector_count;
    quint8 *all_buffer = (quint8* )malloc(sizeof(quint8) * (100*ONE_PACKET_LENGTH + 1));

    memcpy(all_buffer, right_buffer, right_length);
    /*
    for (quint64 i = 0; i < vector_count*ONE_PACKET_LENGTH; i ++) {
        all_buffer[i] = socket_buffer[header_vector_table[0] + i];
    }
    */
    memcpy(all_buffer + right_length, socket_buffer, first_header_index);
    //qDebug() << "join case1!";
    case_1(all_buffer, length);
    unknown_length = length - first_header_index;
    memcpy(unknown_buffer, socket_buffer + first_header_index, unknown_length);

    int ret = check_packet(unknown_buffer,unknown_length);
    if (ret == 1) {
        qDebug() << "join case 1";
        case_1(unknown_buffer,unknown_length);
    }else if( ret == 2 ) {
        qDebug() << "join case 2";
        case_2(unknown_buffer, unknown_length, left_rom, &left_length);
    }else{
        qDebug() << "You are wrong about case not 1 or 2 what else?";
    }
    free(all_buffer);
    delete unknown_buffer;
}

void NetClientThread::case_4(quint8 *socket_buffer, quint64 length)
{
    //qDebug() << "do case 4";
    quint64 first_header_index = 0;
    quint8 *unknown_buffer = new quint8[ONE_PACKET_LENGTH*100];
    quint32 unknown_length = 0;
    quint32 unknown_pac_num = 0;

    for (quint64 i = 0; i < length; i ++) {
        if ( (*(socket_buffer + i) == 0xAD) && (*(socket_buffer + i + 1) == 0xAC) ) {
            first_header_index = i;
            break;
        }
    }
    quint8 *all_buffer = (quint8* )malloc(sizeof(quint8) * (100*ONE_PACKET_LENGTH + 1));

    memcpy(all_buffer, socket_buffer + first_header_index, length - first_header_index + 1);

    int ret = check_packet(unknown_buffer, unknown_length);
    if (ret == 1) {
        //qDebug() << "join case 1";
        case_1(unknown_buffer,unknown_length);
    }else if( ret == 2 ) {
       // qDebug() << "join case 2";
        case_2(unknown_buffer, unknown_length, left_rom, &left_length);
    }else{
        qDebug() << "You are wrong about case not 1 or 2 what else?";
    }
    free(all_buffer);
    delete unknown_buffer;
}



void NetClientThread::on_net_close_file()
{
    if (file_ctr->file->isOpen()) {
        file_ctr->fileClose();
    }
}

void NetClientThread::on_net_enable_save(bool state)
{
    isEnableSave = state;
    if (file_ctr->file->isOpen() && isEnableSave == false) {
        file_ctr->fileClose();
    }

    if (!file_ctr->file->isOpen() && isEnableSave == true) {
    }
}

void NetClientThread::on_file_manager_add_doc_list(QString filename)
{
    emit net_add_doc_list(filename);
}

void NetClientThread::on_file_manager_file_size(double percent)
{
    emit net_file_size(percent);
}

void NetClientThread::stop()
{
    stop();
}

void NetClientThread::enable_socket_read(bool state)
{
    is_enable_socket_read = state;
}