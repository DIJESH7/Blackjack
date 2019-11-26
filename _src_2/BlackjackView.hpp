



class BlackjackView
{
public:
	BlackjackView(chat_client controller)
	{

	}

	void showChanges() 
	{
        revalidate();
        repaint();
        setVisible(true);
    }

    void createWelcomePanel() 
    {
        
        cout << "Welcome to Blackjack!" << endl;
        welcomePanel.add(welcomeLabel, constraints);
        cout << "Waiting for other players to join." << endl; // .setVisible(false)
    }

    void setWelcomeWaiting(bool b) {
        //welcomeWaitingLabel.setVisible(b);
        showChanges();
    }

	setWelcomeWaiting(bool b)
	{
		//welcomeWaitingLabel -> setVisible(b);
		showChanges();
	}

	void showWelcomePanel() 
	{
        showPanel(PanelNames.WelcomePanel);
    }

    void showTurnPanel() 
    {
        showPanel(PanelNames.TurnPanel);
    }

	void showContinuePlayingPanel() 
	{
        showPanel(PanelNames.ContinuePlayingPanel);
    }

 	void reset() 
 	{
        createPanels();
        setupActionListeners();
        showContinuePlayingPanel();
    }

    void actionPerformed(ActionEvent e) {
        Object target = e.getSource();
        if (target == betField || target == betButton) {
            controller.sendClientMessage(betField.getText());
            enableBetField(false);
            enableBetButton(false);
        } else if (target == yesButton) {
            controller.sendClientMessage(yesButton.getText());
            enableYesButton(false);
            enableNoButton(false);
        } else if (target == noButton) {
            controller.sendClientMessage(noButton.getText());
            enableYesButton(false);
            enableNoButton(false);
        }
    }





	chat_client controller;



private:
	enum class PanelNames
	{
		WelcomePanel, BetPanel, TurnPanel, ContinuePlayingPanel
	}

	void showPanel(PanelNames panelName) 
	{
        //CardLayout cardLayout = (CardLayout) getContentPane().getLayout();
        //cardLayout.show(getContentPane(), panelName.toString());
        showChanges();
    }






	
};