package competitive.programming.genetic;

/**
 * @author Manwe
 *
 * Interface allowing to merge two genotype in order to build a new one combination of the two genotype characteristics
 *
 * @param <Genotype>
 *  The class representing one candidate
 */
public interface CandidateMerger<Genotype> {
    /**
     * @param first
     * 	the first instance of genotype
     * @param second
     *  the second instance of genotype
     * @return
     *  a new instance that has been created from the first and second genotype
     */
    Genotype merge(Genotype first, Genotype second);
}
