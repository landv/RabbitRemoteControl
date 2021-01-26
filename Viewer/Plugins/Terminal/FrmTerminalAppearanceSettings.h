#ifndef FRMTERMINALAPPEARANCESETTINGS_H
#define FRMTERMINALAPPEARANCESETTINGS_H

#include <QWidget>
#include "ParameterTerminalAppearance.h"

namespace Ui {
class CFrmTerminalAppearanceSettings;
}

class CFrmTerminalAppearanceSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit CFrmTerminalAppearanceSettings(CParameterTerminalAppearance* pPara, QWidget *parent = nullptr);
    virtual ~CFrmTerminalAppearanceSettings() override;

    int AcceptSettings();

private slots:
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_spFontSize_valueChanged(int size);

private:
    Ui::CFrmTerminalAppearanceSettings *ui;
    CParameterTerminalAppearance *m_pPara;

};

#endif // FRMTERMINALAPPEARANCESETTINGS_H
