#ifndef PBCPLAYVIEW_H
#define PBCPLAYVIEW_H

#include "gui/pbcGridIronView.h"
#include "models/pbcPlay.h"
#include <string>

class PBCPlayView : public PBCGridIronView {
 public:
    explicit PBCPlayView(QObject *parent = 0);
    void repaint();
    void createNewPlay(const std::string& name,
                       const std::string& codeName,
                       const std::string& formationName = "");
    void showPlay(const std::string &name);
    void savePlay(const std::string& name = "",
                  const std::string& codeName = "");
    void saveFormation(const std::string& formationName = "");
    QStringList getAvailableFormations() const;
    QStringList getAvailablePlays() const;

 private:
    PBCPlaySP _currentPlay;
};

#endif  // PBCPLAYVIEW_H
