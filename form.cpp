#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);

}

Form::~Form()
{
    delete ui;
}

void Form::update(){
    Global::EnvFactor evn1=EcologyCore::GetInstance()->GetLayerEnvironment(1);
    ui->progressBar_tepm->setValue(evn1.temp);
    ui->progressBar_pH->setValue(evn1.pH);
    ui->progressBar_pH->setMaximum(14);
    ui->progressBar_oxy->setValue(evn1.oxygen);
    ui->progressBar_osm->setValue(evn1.osmotic);
    ui->progressBar_toxin->setValue(evn1.toxin);
    ui->progressBar_nur->setValue(evn1.nutrition);

    Global::EnvFactor evn2=EcologyCore::GetInstance()->GetLayerEnvironment(2);
    ui->progressBar_tepm_2->setValue(evn2.temp);
    ui->progressBar_pH_2->setValue(evn2.pH);
    ui->progressBar_pH_2->setMaximum(14);
    ui->progressBar_oxy_2->setValue(evn2.oxygen);
    ui->progressBar_osm_2->setValue(evn2.osmotic);
    ui->progressBar_toxin_2->setValue(evn2.toxin);
    ui->progressBar_nur_2->setValue(evn2.nutrition);

    Global::EnvFactor evn3=EcologyCore::GetInstance()->GetLayerEnvironment(1);
    ui->progressBar_tepm_3->setValue(evn3.temp);
    ui->progressBar_pH_3->setValue(evn3.pH);
    ui->progressBar_pH_3->setMaximum(14);
    ui->progressBar_oxy_3->setValue(evn3.oxygen);
    ui->progressBar_osm_3->setValue(evn3.osmotic);
    ui->progressBar_toxin_3->setValue(evn3.toxin);
    ui->progressBar_nur_3->setValue(evn3.nutrition);

    Global::EnvFactor evn4=EcologyCore::GetInstance()->GetLayerEnvironment(1);
    ui->progressBar_tepm_4->setValue(evn4.temp);
    ui->progressBar_pH_4->setValue(evn4.pH);
    ui->progressBar_pH_4->setMaximum(14);
    ui->progressBar_oxy_4->setValue(evn4.oxygen);
    ui->progressBar_osm_4->setValue(evn4.osmotic);
    ui->progressBar_toxin_4->setValue(evn4.toxin);
    ui->progressBar_nur_4->setValue(evn4.nutrition);

    Global::EnvFactor evn5=EcologyCore::GetInstance()->GetLayerEnvironment(1);
    ui->progressBar_tepm_5->setValue(evn5.temp);
    ui->progressBar_pH_5->setValue(evn5.pH);
    ui->progressBar_pH_5->setMaximum(14);
    ui->progressBar_oxy_5->setValue(evn5.oxygen);
    ui->progressBar_osm_5->setValue(evn5.osmotic);
    ui->progressBar_toxin_5->setValue(evn5.toxin);
    ui->progressBar_nur_5->setValue(evn5.nutrition);

    Global::EnvFactor evn6=EcologyCore::GetInstance()->GetLayerEnvironment(1);
    ui->progressBar_tepm_6->setValue(evn6.temp);
    ui->progressBar_pH_6->setValue(evn6.pH);
    ui->progressBar_pH_6->setMaximum(14);
    ui->progressBar_oxy_6->setValue(evn6.oxygen);
    ui->progressBar_osm_6->setValue(evn6.osmotic);
    ui->progressBar_toxin_6->setValue(evn6.toxin);
    ui->progressBar_nur_6->setValue(evn6.nutrition);

}