package competitive.programming.genetic;

/**
 * @author Manwe
 * 
 * Interface allowing to create a new instance of a genotype but that has been mutated
 * 
 * @param <Genotype>
 *  The class representing one candidate
 */
public interface CandidateMutator<Genotype> {
    /**
     * @param candidate
     *  the source instance that will be mutated
     * @return
     *  a new instance that has been created from the candidate and modified
     */
    Genotype mutate(Genotype candidate);
}
