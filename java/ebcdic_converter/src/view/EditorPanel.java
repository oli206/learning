package view;

import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import model.Converter;
import net.miginfocom.swing.MigLayout;

public class EditorPanel extends JPanel implements ActionListener {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private JTextArea textArea;
	private JScrollPane scrollPaneTextArea;
	private JFileChooser fileChooser;
	private JButton buttonBrowse;
	private JTextField textField;
	private JButton buttonSave;

	private File fileToEdit;

	private boolean canSave;

	private Converter converter;

	public EditorPanel(Converter _converter) throws IOException
	{
		super(new MigLayout("fill"));

		converter = _converter;

		textArea = new JTextArea();		
		scrollPaneTextArea = new JScrollPane(textArea, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

		
		fileChooser = new JFileChooser();
		fileChooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
		fileChooser.setCurrentDirectory(new File(new File(".").getCanonicalPath()));
		
		textField = new JTextField("Select file");
		textField.setFont(new Font(textField.getFont().getName(),Font.ITALIC,textField.getFont().getSize()));
		
		buttonBrowse = new JButton("Browse");
		buttonSave = new JButton("Save");

		buttonBrowse.addActionListener(this);
		buttonSave.addActionListener(this);
		
		canSave = false;

		this.add(scrollPaneTextArea,"span 3, wrap, wmin 1000, hmin 600");
		this.add(buttonBrowse, "hmin 40");
		this.add(textField, "hmin 40, growx, push"); 
		this.add(buttonSave, "hmin 40");
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		try {

			if (e.getSource() == buttonBrowse)
			{
				int returnVal = fileChooser.showOpenDialog(this);

				if (returnVal == JFileChooser.APPROVE_OPTION)
				{
					fileToEdit = new File(fileChooser.getSelectedFile().getPath());

					if (fileToEdit.exists() && fileToEdit.isFile())
					{
						canSave = true;
						textArea.setText(converter.readFile(fileToEdit));
						textField.setFont(new Font(textField.getFont().getName(),Font.PLAIN,textField.getFont().getSize()));
						textField.setText(fileToEdit.getAbsolutePath());
					}
					else 
					{
						canSave = false;
						textField.setFont(new Font(textField.getFont().getName(),Font.ITALIC,textField.getFont().getSize()));
						textField.setText("Select file");
					}
				}
			}
			else if (e.getSource() == buttonSave)
			{
				if (canSave)
				{
					File destinationFile = new File(fileToEdit.getPath() + "_converted");
					converter.convert(destinationFile, textArea.getText());
				}
			}
		} 
		catch (IOException exception) 
		{
			JOptionPane.showMessageDialog(this,"Couldn't read/write file","IO Error",JOptionPane.ERROR_MESSAGE);
		}
	}



}
