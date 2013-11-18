Public Class Form1

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Try
            With SerialPort1
                .PortName = "COM7"  '<<<< Change this number for your computer
                .BaudRate = 96000
                .Parity = IO.Ports.Parity.None
                .DataBits = 8
                .StopBits = IO.Ports.StopBits.One
                .ReadTimeout = 300  '300ms
                .WriteTimeout = 300 '300ms
            End With
            SerialPort1.Open()
            Label1.Text = "Connected"
        Catch ex As Exception
            MsgBox(ex.ToString)
        End Try
    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        Dim byteOut(64) As Byte
        Dim i As Integer
        Try
            byteOut(0) = Len(TextBox1.Text) + 5  'number bytes in output message
            byteOut(1) = &H0  'should be 0 for NXT
            byteOut(2) = &H80 '&H0 = reply expected &H80 = no reply expected
            byteOut(3) = &H9  'Send Bluetooth
            byteOut(4) = &H0  'Box Number - 1
            byteOut(5) = Len(TextBox1.Text) + 1  'message size with null terminator
            For i = 1 To Len(TextBox1.Text) 'copy bytes into output array
                byteOut(i + 5) = Asc(Mid(TextBox1.Text, i, 1))
            Next
            byteOut(Len(TextBox1.Text) + 6) = &H0 'add null terminator
            SerialPort1.Write(byteOut, 0, Len(TextBox1.Text) + 7) 'send message
        Catch ex As Exception
            MsgBox(ex.ToString)
        End Try
    End Sub

    Private Sub Timer1_Tick(sender As Object, e As EventArgs) Handles Timer1.Tick
        Dim byteOut(64) As Byte
        Dim i As Integer
        Try
            byteOut(0) = Len(TextBox1.Text) + 5  'number bytes in output message
            byteOut(1) = &H0  'should be 0 for NXT
            byteOut(2) = &H80 '&H0 = reply expected &H80 = no reply expected
            byteOut(3) = &H9  'Send Bluetooth
            byteOut(4) = &H0  'Box Number - 1
            byteOut(5) = Len(TextBox1.Text) + 1  'message size with null terminator
            For i = 1 To Len(TextBox1.Text) 'copy bytes into output array
                byteOut(i + 5) = Asc(Mid(TextBox1.Text, i, 1))
            Next
            byteOut(Len(TextBox1.Text) + 6) = &H0 'add null terminator
            SerialPort1.Write(byteOut, 0, Len(TextBox1.Text) + 7) 'send message
        Catch ex As Exception
            MsgBox(ex.ToString)
        End Try
    End Sub
End Class
