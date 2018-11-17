import jamspell


class SpellCorrectorRus:
    def __init__(self):
        self.corrector = jamspell.TSpellCorrector()
        self.corrector.LoadLangModel('ru_small.bin')

    def correct(self, string):
        return self.corrector.FixFragment(string)