# TIE-02101 Johdanto ohjelmointiin
# Julia Harttunen, julia.harttunen@tuni.fi
# Krista Mätäsniemi, krista.matasniemi@tuni.fi
# Projekti 13: Ristinolla-peli

from tkinter import *

PELAAJIEN_MAARA = 2

class pelilauta:

    def __init__(self):
        self.__ikkuna = Tk()
        self.__ikkuna.title("Ristinolla")

        self.__vuorot = 0
        self.__vari = ""
        self.__pelaaja = ""

        # Luodaan pelaajien nimien kyselykentät

        self.__pelaaja1 = Entry()
        self.__pelaaja1.grid(row=8, column=1, columnspan=4)
        self.__pelaaja2 = Entry()
        self.__pelaaja2.grid(row=8, column=5, columnspan=4)

        # Luodaan pelin ilmoitusteksti

        self.__yla_teksti = Label(self.__ikkuna)
        self.__yla_teksti.grid(row=0, column=1, columnspan=8, padx=15, pady=15)

        # Luodaan pelaaja tekstit

        self.__tekstikentta1 = Label(self.__ikkuna, text="Pelaaja 1:")
        self.__tekstikentta1.grid(row=7, column=1, columnspan=4, sticky=E+W, padx=15, pady=15)
        self.__tekstikentta2 = Label(self.__ikkuna, text="Pelaaja 2:")
        self.__tekstikentta2.grid(row=7, column=5, columnspan=4, sticky=E+W, padx=15, pady=15)

        # Luodaan pelin aloitusnappi

        self.__aloita_peli = Button(self.__ikkuna, text="Aloita peli!", command=self.uusi_peli)
        self.__aloita_peli.grid(row=9, column=3, columnspan=4, sticky=E+W, padx=15, pady=15)

    def kaynnistys(self):
        """
        Avaa peli-ikkunan.
        :return:
        """
        self.__ikkuna.mainloop()

    def uusi_peli(self):
        """
        Luodaan uusi peli.
        :return:
        """
        luku = [-1]

        # Luodaan pelilaudan pelinapit.

        self.__napit = []

        for i in range(3):

            for j in range(3):
                rivi = (2 * j) + 1
                sarake = (2 * i) + 2
                nappi = Button(self.__ikkuna, bg="white", state=NORMAL)

                def ruudun_valinta():
                    luku[0] += 1
                    return self.lisaa_komento(luku[0])

                nappi.grid(row=rivi, column=sarake, rowspan=2, columnspan=2,
                           sticky=E+W+N+S, padx=5, pady=5)
                self.__napit.append(nappi)
                ruudun_valinta()

        self.vuoron_tarkistus()

    def lisaa_komento(self, indeksi):
        """
        Lisää toiminnot pelinappeihin.
        :param indeksi: Pelinapin indeksin arvo.
        :return:
        """
        def muuta_vari():
            """
            Pelinapin toiminta.
            :return:
            """
            nappi = self.__napit[indeksi]

            if self.__vuorot % PELAAJIEN_MAARA == 0:
                if nappi.configure("bg")[-1] == "white":
                    nappi.configure(bg="pink", state=DISABLED)

            else:
                if nappi.configure("bg")[-1] == "white":
                    nappi.configure(bg="lightblue", state=DISABLED)

            # Tarkistetaan pelin tilanne.

            voitto = self.voiton_tarkastelu()

            if voitto == False:
                self.__vuorot += 1
                self.vuoron_tarkistus()

        self.__napit[indeksi].configure(command=muuta_vari)

    def vuoron_tarkistus(self):
        """
        Vuoron vaihto.
        :return:
        """
        pelaaja1 = self.__pelaaja1.get()
        pelaaja2 = self.__pelaaja2.get()

        if self.__vuorot % PELAAJIEN_MAARA == 0:
            self.__pelaaja = pelaaja1
            self.__vari = "pink"

        else:
            self.__pelaaja = pelaaja2
            self.__vari = "lightblue"

        self.__yla_teksti.configure(text="Pelaajan " + str(self.__pelaaja)
                                         + " vuoro!")

    def voiton_tarkastelu(self):
        """
        Tarkistetaan pelin tilanne.
        :return: True, jos pelille on voittaja tai False, jos peli jatkuu.
        """
        voitto = 0

        # Tarkastetaan vaakarivit.

        for j in range(3):
            vaaka = 0
            for i in range(3):
                indeksi = (3*i)+j
                if self.__napit[indeksi].configure("bg")[-1] == self.__vari:
                    vaaka += 1
                    if vaaka == 3:
                        voitto += 1

        # Tarkastetaan pystyrivit.

        for j in range(3):
            pysty = 0
            for i in range(3):
                indeksi = i+(j*3)
                if self.__napit[indeksi].configure("bg")[-1] == self.__vari:
                    pysty += 1
                    if pysty == 3:
                        voitto += 1

        # Tarkastetaan vinorivit.

        vino = 0
        for i in range(3):
            indeksi = 4*i
            if self.__napit[indeksi].configure("bg")[-1] == self.__vari:
                vino += 1
                if vino == 3:
                    voitto += 1

        vino = 0
        for i in range(3):
            indeksi = (2*i)+2
            if self.__napit[indeksi].configure("bg")[-1] == self.__vari:
                vino += 1
                if vino == 3:
                    voitto += 1

        # Tutkitaan, onko voittajaa.

        if voitto > 0:
            self.__yla_teksti.configure(
                text="Pelaaja " + str(self.__pelaaja) + " voitti!!!")

            for nappi in self.__napit:
                nappi.configure(state=DISABLED)

            return True

        else:
            epatosi = self.tasapelin_tarkistus()
            return epatosi

    def tasapelin_tarkistus(self):
        """
        Tutkitaan, onko kaikki ruudut käytetty.
        :return: False, jos löytyy yksikin vapaa ruutu.
        """
        taynna = 0

        for nappi in self.__napit:
            if nappi.configure("bg")[-1] == "white":
                return False

            else:
                taynna += 1
                if taynna == 9:
                    self.__yla_teksti.configure(text="Tasapeli!!!")
                    for nappi in self.__napit:
                        nappi.configure(state=DISABLED)


def main():
    peli = pelilauta()
    peli.kaynnistys()

main()