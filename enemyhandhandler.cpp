#include "enemyhandhandler.h"
#include <QtWidgets>

EnemyHandHandler::EnemyHandHandler(QObject *parent, QMap<QString, QJsonObject> *cardsJson, Ui::MainWindow *ui) : QObject(parent)
{
    this->ui = ui;
    this->cardsJson = cardsJson;
    this->inGame = false;
}


EnemyHandHandler::~EnemyHandHandler()
{
    ui->enemyHandListWidget->clear();
    enemyHandList.clear();
}


void EnemyHandHandler::reset()
{
    ui->enemyHandListWidget->clear();
    enemyHandList.clear();
}


void EnemyHandHandler::showEnemyCardDraw(int id, int turn, bool special, QString code)
{
    HandCard handCard(cardsJson);
    handCard.id = id;
    handCard.turn = turn;
    handCard.special = special;
    handCard.code = code;
    handCard.listItem = new QListWidgetItem();
    ui->enemyHandListWidget->addItem(handCard.listItem);

    handCard.draw();
    enemyHandList.append(handCard);

    showCount();

    if(code != "")      emit checkCardImage(code);
}


void EnemyHandHandler::showCount()
{
    if(inGame && ui->tabWidget->currentIndex()==tabEnemy)
    {
        emit sendStatusBarMessage(tr("Enemy hand: ") + QString::number(enemyHandList.count()));
    }
}


void EnemyHandHandler::lastHandCardIsCoin()
{
    if(enemyHandList.empty())   return;//En modo practica el mulligan enemigo termina antes de robar las cartas
    enemyHandList.last().code = COIN;
    enemyHandList.last().draw();

    emit checkCardImage(COIN);
}


void EnemyHandHandler::showEnemyCardPlayed(int id, QString code)
{
    (void) code;

    int i=0;
    for (QList<HandCard>::iterator it = enemyHandList.begin(); it != enemyHandList.end(); it++, i++)
    {
        if(it->id == id)
        {
            delete it->listItem;
            enemyHandList.removeAt(i);
            showCount();
            return;
        }
    }
}


void EnemyHandHandler::redrawDownloadedCardImage(QString code)
{
    for (QList<HandCard>::iterator it = enemyHandList.begin(); it != enemyHandList.end(); it++)
    {
        if(it->code == code)    it->draw();
    }
}


void EnemyHandHandler::lockEnemyInterface()
{
    ui->tabEnemy->setAttribute(Qt::WA_NoBackground);
    ui->tabEnemy->repaint();

    reset();
    this->inGame = true;
}


void EnemyHandHandler::unlockEnemyInterface()
{
    ui->tabEnemy->setAttribute(Qt::WA_NoBackground, false);
    ui->tabEnemy->repaint();

    this->inGame = false;
}